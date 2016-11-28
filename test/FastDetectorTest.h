#ifndef __FASTDETECTOR_TEST_H
#define __FASTDETECTOR_TEST_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <chrono>
#include "../../modules/vistracker/src/fast.h"

namespace atlas
{
namespace visual
{

class FastDetectorTest : public ::testing::Test
{
public:
	void SetUp() override {
		m_frame_extractor = BenchmarkEventProvider::Ptr (new BenchmarkEventProvider (dataset_path));
		timeCV = 0;
		timeOP = 0;
		frameListener = m_frame_extractor->event_frame_ready.createListener(
		  [this](util::BGRAFrame::Ptr frame) {
			this->extractFeaturesCallback(frame);
		});
	}
	
	// @brief returns difference in number of keypoints
	// from SRI-B and OpenCV implementations
	int testNKeypoints() {
		int difference = 0;
		auto gtItr = keypoints_gt.begin();
		auto esItr = keypoints_calc.begin();
		for(; gtItr != keypoints_gt.end() & esItr != keypoints_calc.end(); ++gtItr, ++esItr) {
			difference += std::abs(gtItr->size() - esItr->size());
		}
		return difference;
	}
	
	void TearDown() override {

	}

	void testLaunch() {
		m_frame_extractor->run();
		m_frame_extractor->stop();
		std::cout << "Original Fast Time: " << timeCV*1e-3 << " ms\n";
		std::cout << "Optimized Fast Time: " << timeOP*1e-3 << " ms\n";
	}

	void extractFeaturesCallback(util::BGRAFrame::Ptr frame) {
		std::vector<cv::KeyPoint> kpt_gt;
		std::vector<cv::KeyPoint> kpt_es;
		cv::Mat pyrImg;
		cv::cvtColor (frame->get(), pyrImg, cv::COLOR_BGRA2GRAY);
		for(int i=0; i<LEVELS; ++i) {
			if(i!=0) {
				cv::Mat pyrImgDown;
				cv::pyrDown(pyrImg, pyrImgDown);
				pyrImg = pyrImgDown;
			}
			int fastThreshold_ = 0;
			switch (i) {
			case 0: fastThreshold_ = 10; break;
			case 1: fastThreshold_ = 15; break;
			case 2: fastThreshold_ = 15; break;
			default: fastThreshold_ = 10;
			}
			std::vector<cv::KeyPoint> kpts_gt_l, kpts_es_l;
			auto startCV = std::chrono::high_resolution_clock::now();
			cv::FAST(pyrImg, kpts_gt_l, fastThreshold_, false);
			timeCV += (double)std::chrono::duration_cast<std::chrono::microseconds>(
					std::chrono::high_resolution_clock::now()-startCV).count();
			for(auto kItr : kpts_gt_l) {
				kpt_gt.push_back(kItr);
			}
			auto startOP = std::chrono::high_resolution_clock::now();
			atlas::visual::FAST_(pyrImg, kpts_es_l, fastThreshold_, false);
			timeOP += (double)std::chrono::duration_cast<std::chrono::microseconds>(
								std::chrono::high_resolution_clock::now()-startOP).count();
			for(auto kItr : kpts_es_l) {
				kpt_es.push_back(kItr);
			}
		}
		keypoints_gt.push_back(kpt_gt);
		keypoints_calc.push_back(kpt_es);
	}
	

	
	
private:
	EventListener<util::BGRAFrame::Ptr> frameListener;
	BenchmarkEventProvider::Ptr m_frame_extractor;
	static const int LEVELS = 4;
	double timeCV;
	double timeOP;
	std::vector< std::vector<cv::KeyPoint> > keypoints_gt;
	std::vector< std::vector<cv::KeyPoint> > keypoints_calc;
};

} // namespace visual
} // namespace atlas

#endif
