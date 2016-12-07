/**
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */

#ifndef CONTAINER_ERASER
#define CONTAINER_ERASER

#include <algorithm>
#include <functional>
#include "TypeTraits.h"

namespace Tools
{

/* DEFINITIONS */

template<class Container>
void Erase(Container* container, std::function<bool(const typename Container::value_type&)> functor);

template<class Container>
void EraseLess(Container* container, const typename Container::value_type& threshold);

template<class Container>
void EraseGreater(Container* container, const typename Container::value_type& threshold);

template<class Container>
void EraseNotLess(Container* container, const typename Container::value_type& threshold);

template<class Container>
void EraseNotGreater(Container* container, const typename Container::value_type& threshold);

template<class Container>
void EraseMapValueLess(Container* container, const typename Container::mapped_type& threshold);

template<class Container>
void EraseMapValueGreater(Container* container, const typename Container::mapped_type& threshold);

template<class Container>
void EraseMapValueNotLess(Container* container, const typename Container::mapped_type& threshold);

template<class Container>
void EraseMapValueNotGreater(Container* container, const typename Container::mapped_type& threshold);

template<class Container0, class Container1>
void EraseDifference(Container0* container0, const Container1& container1);

template<class Container0, class Container1>
void Difference(Container0* container0, const Container1& container1);

template<class Container0, class Container1>
Container0 Difference(const Container0& container0, const Container1& container1);

template<class Container0, class Container1>
void Intersection(Container0* container0, const Container1& container1);

template<class Container0, class Container1>
Container1 Intersection(const Container0& container0, const Container1& container1);

template<class Container>
void Distinct(Container* container);

template<class Container>
Container Distinct(const Container& container);

/* IMPLEMENTATION */

namespace ContainerEraser
{
    template<class Container>
    void EraseVector(Container* container, std::function<bool(const typename Container::value_type&)> functor)
    {
        container->erase(std::remove_if(container->begin(), container->end(), functor), container->end());
    }

    template<class Container>
    void EraseMap(Container* container, std::function<bool(const typename Container::value_type&)> functor)
    {
        for (auto iter = container->begin(); iter != container->end();)
        {
            if (functor(*iter)) {
                container->erase(iter++);
            } else {
                ++iter;
            }
        }
    }

    template<int ContainerType>
    struct Eraser
    {
        template<class Container>
        static void Erase(Container* container, std::function<bool(const typename Container::value_type&)> functor);
    };

    template<>
    struct Eraser<Tools::UNARY>
    {
        template<class Container>
        static void Erase(Container* container, std::function<bool(const typename Container::value_type&)> functor)
        {
            EraseVector(container, functor);
        }
    };

    template<>
    struct Eraser<Tools::COUPLING>
    {
        template<class Container>
        static void Erase(Container* container, std::function<bool(const typename Container::value_type&)> functor)
        {
            EraseMap(container, functor);
        }
    };
}

namespace EraserDifference
{
    template<class Container0, class Container1>
    void EraseDifferenceVectorVector(Container0* container0, const Container1& container1)
    {
        std::function<bool(const typename Container0::value_type&)> functor =
            [&](const typename Container0::value_type& val)
            {
                return std::find(container1.begin(), container1.end(), val) != container1.end();
            };
        Tools::Erase(container0, functor);
    }

    template<class Container0, class Container1>
    void EraseDifferenceVectorMap(Container0* container0, const Container1& container1)
    {
        std::function<bool(const typename Container0::value_type&)> functor =
            [&](const typename Container0::value_type& val)
            {
                return container1.find(val) != container1.end();
            };
        Tools::Erase(container0, functor);
    }

    template<class Container0, class Container1>
    void EraseDifferenceMapMap(Container0* container0, const Container1& container1)
    {
        std::function<bool(const typename Container0::value_type&)> functor =
            [&](const typename Container0::value_type& val)
            {
                return container1.find(val.first) != container1.end();
            };
        Tools::Erase(container0, functor);
    }

    template<class Container0, class Container1>
    void EraseDifferenceMapVector(Container0* container0, const Container1& container1)
    {
        std::function<bool(const typename Container0::value_type&)> functor =
            [&](const typename Container0::value_type& val)
            {
                return std::find(container1.begin(), container1.end(), val.first) != container1.end();
            };
        Tools::Erase(container0, functor);
    }

    template<int ContainerType0, int ContainerType1>
    struct Eraser
    {
        template<class Container0, class Container1>
        static void Erase(Container0* container0, const Container1& container1);
    };

    template<>
    struct Eraser<Tools::UNARY, Tools::UNARY>
    {
        template<class Container0, class Container1>
        static void Erase(Container0* container0, const Container1& container1)
        {
            EraseDifferenceVectorVector(container0, container1);
        }
    };

    template<>
    struct Eraser<Tools::UNARY, Tools::COUPLING>
    {
        template<class Container0, class Container1>
        static void Erase(Container0* container0, const Container1& container1)
        {
            EraseDifferenceVectorMap(container0, container1);
        }
    };

    template<>
    struct Eraser<Tools::COUPLING, Tools::UNARY>
    {
        template<class Container0, class Container1>
        static void Erase(Container0* container0, const Container1& container1)
        {
            EraseDifferenceMapVector(container0, container1);
        }
    };

    template<>
    struct Eraser<Tools::COUPLING, Tools::COUPLING>
    {
        template<class Container0, class Container1>
        static void Erase(Container0* container0, const Container1& container1)
        {
            EraseDifferenceMapMap(container0, container1);
        }
    };

}

namespace EraserIntersection
{
    template<class Container0, class Container1>
    void EraseInersectionVectorVector(Container0* container0, const Container1& container1)
    {
        std::function<bool(const typename Container0::value_type&)> functor =
            [&](const typename Container0::value_type& val)
            {
                return std::find(container1.begin(), container1.end(), val) == container1.end();
            };
        Tools::Erase(container0, functor);
    }

    template<class Container0, class Container1>
    void EraseInersectionVectorMap(Container0* container0, const Container1& container1)
    {
        std::function<bool(const typename Container0::value_type&)> functor =
            [&](const typename Container0::value_type& val)
            {
                return container1.find(val) == container1.end();
            };
        Tools::Erase(container0, functor);
    }

    template<class Container0, class Container1>
    void EraseInersectionMapMap(Container0* container0, const Container1& container1)
    {
        std::function<bool(const typename Container0::value_type&)> functor =
            [&](const typename Container0::value_type& val)
            {
                return container1.find(val.first) == container1.end();
            };
        Tools::Erase(container0, functor);
    }

    template<class Container0, class Container1>
    void EraseInersectionMapVector(Container0* container0, const Container1& container1)
    {
        std::function<bool(const typename Container0::value_type&)> functor =
            [&](const typename Container0::value_type& val)
            {
                return std::find(container1.begin(), container1.end(), val.first) == container1.end();
            };
        Tools::Erase(container0, functor);
    }

    template<int ContainerType0, int ContainerType1>
    struct Eraser
    {
        template<class Container0, class Container1>
        static void Erase(Container0* container0, const Container1& container1);
    };

    template<>
    struct Eraser<Tools::UNARY, Tools::UNARY>
    {
        template<class Container0, class Container1>
        static void Erase(Container0* container0, const Container1& container1)
        {
            EraseInersectionVectorVector(container0, container1);
        }
    };

    template<>
    struct Eraser<Tools::UNARY, Tools::COUPLING>
    {
        template<class Container0, class Container1>
        static void Erase(Container0* container0, const Container1& container1)
        {
            EraseInersectionVectorMap(container0, container1);
        }
    };

    template<>
    struct Eraser<Tools::COUPLING, Tools::UNARY>
    {
        template<class Container0, class Container1>
        static void Erase(Container0* container0, const Container1& container1)
        {
            EraseInersectionMapVector(container0, container1);
        }
    };

    template<>
    struct Eraser<Tools::COUPLING, Tools::COUPLING>
    {
        template<class Container0, class Container1>
        static void Erase(Container0* container0, const Container1& container1)
        {
            EraseInersectionMapMap(container0, container1);
        }
    };

}


template<class Container>
void Erase(Container* container, std::function<bool(const typename Container::value_type&)> functor)
{
    ContainerEraser::Eraser<Tools::TypeIdentifier<Container>::containerType>::Erase(container, functor);
}

template<class Container>
void EraseLess(Container* container, const typename Container::value_type& threshold)
{
    Erase<Container>(container, [&](const typename Container::value_type& val) { return val < threshold; });
}

template<class Container>
void EraseGreater(Container* container, const typename Container::value_type& threshold)
{
    Erase<Container>(container, [&](const typename Container::value_type& val) { return val > threshold; });
}

template<class Container>
void EraseNotLess(Container* container, const typename Container::value_type& threshold)
{
    Erase<Container>(container, [&](const typename Container::value_type& val) { return val >= threshold; });
}

template<class Container>
void EraseNotGreater(Container* container, const typename Container::value_type& threshold)
{
    Erase<Container>(container, [&](const typename Container::value_type& val) { return val <= threshold; });
}

template<class Container>
void EraseMapValueLess(Container* container, const typename Container::mapped_type& threshold)
{
    Erase<Container>(container, [&](const typename Container::value_type& val) { return val.second < threshold; });
}

template<class Container>
void EraseMapValueGreater(Container* container, const typename Container::mapped_type& threshold)
{
    Erase<Container>(container, [&](const typename Container::value_type& val) { return val.second > threshold; });
}

template<class Container>
void EraseMapValueNotLess(Container* container, const typename Container::mapped_type& threshold)
{
    Erase<Container>(container, [&](const typename Container::value_type& val) { return val.second >= threshold; });
}

template<class Container>
void EraseMapValueNotGreater(Container* container, const typename Container::mapped_type& threshold)
{
    Erase<Container>(container, [&](const typename Container::value_type& val) { return val.second <= threshold; });
}

template<class Container0, class Container1>
void Difference(Container0* container0, const Container1& container1)
{
    EraserDifference::Eraser<
        Tools::TypeIdentifier<Container0>::containerType
        ,  Tools::TypeIdentifier<Container1>::containerType
    >::Erase(container0, container1);
}

template<class Container0, class Container1>
void Intersection(Container0* container0, const Container1& container1)
{
    EraserIntersection::Eraser<
        Tools::TypeIdentifier<Container0>::containerType
        ,  Tools::TypeIdentifier<Container1>::containerType
    >::Erase(container0, container1);
}

template<class Container0, class Container1>
Container0 Difference(const Container0& container0, const Container1& container1)
{
    Container0 copy0 = container0;
    Tools::Difference(&copy0, container1);
    return copy0;
}

template<class Container0, class Container1>
Container1 Intersection(const Container0& container0, const Container1& container1)
{
    Container0 copy0 = container0;
    Tools::Intersection(&copy0, container1);
    return copy0;
}

template<class Container>
void Distinct(Container* container)
{
    typedef typename Container::value_type T;
    vector<T> toSort(container->begin(), container->end());
    std::sort(toSort.begin(), toSort.end());
    vector<T> distinct(toSort.size());
    typename vector<T>::iterator end = std::unique_copy(
        toSort.begin(), toSort.end(), distinct.begin());
    *container = Container(distinct.begin(), end);
}

template<class Container>
Container Distinct(const Container& container)
{
    Container distinct = container;
    Distinct(&distinct);
    return distinct;
}

}

#endif // CONTAINER_ERASER

