#ifndef __VISTRACKER_TEST_H
#define __VISTRACKER_TEST_H

#include <opencv2/core/core.hpp>
#include "vistracker.h"
#include "../benchmark/include/benchmark_event_provider.h"
#include <future>
#include <Eigen/SVD>
#include "log.h"
#include <thread>
#include <atomic>

//#define VIRTUAL_METHOD_ERROR_CALL

static char* dataset_path;

namespace atlas
{
namespace visual
{

class VistrackerTest  : public ::testing::Test
{
	typedef util::Pose::Ptr PosePtr;

public:
	VistrackerTest () = default;

	void SetUp() {
		vistracker = VisualTracker::Ptr (new VisualTracker (480,640));
		m_frame_extractor = BenchmarkEventProvider::Ptr (new BenchmarkEventProvider (dataset_path));
		frame_extractor_has_finished.store(false);

		trajectory.open("trajectory.txt");

		vistracker->frame_listener = m_frame_extractor->event_frame_ready.createListener(
				[this](util::BGRAFrame::Ptr frame)	{
			this->vistracker->callBackFrameReceivedSafe (frame, true);
		});

		vistracker->event_terminate_listener = m_frame_extractor->event_terminate.createListener(
		  [this](bool) {
			frame_extractor_has_finished.store(true);
		});

#ifdef VIRTUAL_METHOD_ERROR_CALL
		event_pose_listener_ = vistracker->mpTracker->event_pose_.createListener(
				[this](atlas::util::package::PosePackage::Ptr pose)	{
			auto &p = pose->pose;
			char buf[1024];
			sprintf(buf, "%10.6f ", pose->timestamp);
			this->trajectory << buf << p.t.x() << " " << p.t.y() << " " << p.t.z() << " " << p.q.x() << " " << p.q.y() << " " << p.q.z() << " " << p.q.w() << std::endl;
		});
#endif

		vistracker->event_tracking_listener = m_frame_extractor->event_tracking.createListener(
				[this](bool flag)	{
			D("got an m_frame_extractor->event_tracking event");
			this->vistracker->callbackTrackingCommandReceivedSafe (flag);
		});

		stereo_pose_listener = vistracker->mpTracker->event_stereo_pose_.createListener(
		[this](PosePtr pose) {
			stereo_pose_promise.set_value(pose);

		});
	}

  void TearDown() {

  }

  bool testIfInitialized ()
  {
	  D("VistrackerTest Waiting init");
	  f = stereo_pose_promise.get_future();
	  f.wait();
	  D("VistrackerTest has init");
	  return true;
  }

  /**
   * returns norm of q_ref'*q expected to be 1.0
   */
  double testInitRotation () {
	  auto q = f.get()->pose.q;
	  auto q_ref = m_frame_extractor->getRefInitPose().q;
	  return (q_ref.conjugate() * q).norm();
  }


  /**
   * returns an error of the least squares fit of t to t_ref
   */
  double testInitTranslation () {
	  auto t = f.get()->pose.t;
	  auto t_ref = m_frame_extractor->getRefInitPose().t;

	  std::cout << t.transpose () << std::endl;
	  std::cout << t_ref.transpose () << std::endl;

	  auto x =  t.jacobiSvd (Eigen::ComputeFullU | Eigen::ComputeFullV).solve (t_ref);

	  return (t*x - t_ref).norm();
  }

  void testLaunch () {
	  vistracker->launch();
	  m_frame_extractor->launch();
	  D("modules launched");
  }

  void testStop() {
	  while(!frame_extractor_has_finished) { }
	  m_frame_extractor->stop();
	  vistracker->stop();
	  D("modules stopped");
  }

  // returns SSD of GT and estimated trajectories
  double testTrackFrame() {
	  double ssd = 0;
	  m_frame_extractor->getFrame();
  }

  void setDataSetPath(const std::string &path) {
	  m_dataset_path = path;
  }


private:
  BenchmarkEventProvider::Ptr m_frame_extractor;
  VisualTracker::Ptr vistracker;
  EventListener<util::package::PosePackage::Ptr> event_pose_listener_;

  EventListener<PosePtr> stereo_pose_listener;
  std::promise<PosePtr> stereo_pose_promise;
  std::shared_future<PosePtr> f;
  std::atomic_bool frame_extractor_has_finished;

  std::ofstream trajectory;
  std::string m_dataset_path;


};

}//namespace atlas
}//namespace visual

#endif
