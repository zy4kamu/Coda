/** 
 * @file	Trie.h
 * @brief	prefix tree (trie) data structure
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


#ifndef TRIEDICTIONARY
#define TRIEDICTIONARY

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

#define MAX_DEPTH 100

namespace Tools
{

class ETrieStackOverflow;

/**
 * @class	TrieDictionary
 * @brief	TrieDictionary data structure
 */

template<typename Key, typename Data>
class TrieDictionary
{
    typedef typename Key::value_type KeyAtom;
    public: class TrieIterator;
protected:
    /**
     * @class	TrieNode
     * @brief	TrieNode nested class
     **/
    class TrieNode
    {
        friend class TrieIterator;
    public:
        /**
         * @brief	Constructor of trie
         * @param[in]	i_key - key to be stored in the node
         * @param[in]   extra data to be stored in the node
         */
        TrieNode(const KeyAtom& i_key, const Data& i_value) :
            m_key(i_key)
          , m_data(i_value)
          , m_final(false) {}
        /**
         * @brief	Constructor of trie. Contructs an empty node for the top.
         */
        TrieNode() :
           m_data(0)
         , m_final(false) {}
        /**
         * @brief	Checks if the current node is a list node
         * @returns true if final
         */
        bool final() const {return m_final;}
        /**
         * @brief	Sets the list flag for the node
         * @param[in]	i_isFinal - list flag
         */
        void setFinal(bool i_isFinal) {m_final = i_isFinal;}
        /**
         * @brief	Returns the child node with a specified key
         * @param[in]	i_letter - the key
         * @returns child with the specified key or empty node if it doesn't exist
         */
        std::shared_ptr<TrieNode> getChild(const KeyAtom& i_letter)
        {
            if(m_children.find(i_letter) == m_children.end())
                return std::shared_ptr<TrieNode>();
            return m_children.at(i_letter);
        }
        /**
         * @brief	Checks if the node has a child with a specified key
         * @param[in]	i_letter - key
         * @returns true if the child exists
         */
        bool hasChild(const KeyAtom& i_letter) const {return m_children.find(i_letter) != m_children.end();}
        /**
         * @brief	Adds new child to the node
         * @param[in]	i_letter - key
         * @param[in]	i_data - data
         */
        void addChild(const KeyAtom& i_letter, const Data& i_data)
        {
            if(!hasChild(i_letter))
            {
                std::shared_ptr<TrieNode> newNode = std::make_shared<TrieNode>(i_letter, i_data);
                m_children[i_letter] = newNode;
            }
        }
        /**
         * @brief	Returns data at the node
         * @returns	data at the node
         */
        Data& data() {return m_data;}
        /**
         * @brief	Setd data to the node
         */
        void setData(const Data& i_rhs)
        {
            m_data = i_rhs;
        }

    private:
        std::unordered_map<KeyAtom, std::shared_ptr<TrieNode>> m_children;
        typename Key::value_type m_key;
        Data m_data;
        bool m_final;
        using child_it = typename std::unordered_map<KeyAtom, std::shared_ptr<TrieNode>>::iterator;
    };

public:
    /**
     * @class	TrieEndIterator
     * @brief	End iterator type
     **/
    class TrieEndIterator
    {
    public:
        TrieEndIterator(){}
    };

    /**
     * @class	TrieIterator
     * @brief	Iterator type
     **/
    class TrieIterator
    {
    public:
        /**
         * @brief	Constructor of trie iterator
         * @param[in]	i_start - start node
         */
        TrieIterator(const std::shared_ptr<TrieNode>& i_start)
            : m_ptr(i_start)
            , m_end(false)
            , m_pair(0)
        {
            m_itStack.push_back(std::make_pair<decltype(m_ptr->m_children.begin()), decltype(m_ptr->m_children.end())>(m_ptr->m_children.begin(), m_ptr->m_children.end()));
            m_first.push_back(m_itStack[m_itStack.size()-1].first->second->m_key);
            while(! m_itStack[m_itStack.size()-1].first->second->final())
            {
                std::pair<decltype(m_ptr->m_children.begin()), decltype(m_ptr->m_children.end())> newLevel(m_itStack[m_itStack.size()-1].first->second->m_children.begin(),
                                                                                                           m_itStack[m_itStack.size()-1].first->second->m_children.end());
                m_itStack.push_back(newLevel);
                m_first.push_back(m_itStack[m_itStack.size()-1].first->second->m_key);
                if(m_itStack.size() == MAX_DEPTH)
                {
                    throw ETrieStackOverflow();
                }
            }
        }

        /**
         * @brief	Constructor of trie iterator. Sets iterator at the end position.
         */
        TrieIterator() :
          m_end(true)
        , m_pair(0){}

        /**
         * @brief	Destructor of trie
         */
        ~TrieIterator()
        {
            if(m_pair)
            {
                delete m_pair;
            }
        }

        /**
         * @brief	operator++
         */
        TrieIterator& operator++(int)
        {
            if(m_itStack.empty())
            {
                m_first.clear();
                m_end = true;
                return *this;
            }

            if(!m_itStack[m_itStack.size()-1].first->second->m_children.empty())
            {
                while(1)
                {
                    std::pair<decltype(m_ptr->m_children.begin()), decltype(m_ptr->m_children.end())> newLevel(m_itStack[m_itStack.size()-1].first->second->m_children.begin(),
                                                                                                               m_itStack[m_itStack.size()-1].first->second->m_children.end());
                    m_itStack.push_back(newLevel);
                    m_first.push_back(m_itStack[m_itStack.size()-1].first->second->m_key);
                    if(m_itStack.size() == MAX_DEPTH)
                    {
                        throw ETrieStackOverflow();
                    }
                    if(m_itStack[m_itStack.size()-1].first->second->final())
                    {
                        break;
                    }
                }
                return *this;
            }

            m_itStack[m_itStack.size()-1].first++;

            if (m_itStack[m_itStack.size()-1].first == m_itStack[m_itStack.size()-1].second) // == end() ?
            {
                while(1)
                {
                    m_itStack.pop_back();
                    m_first.pop_back();
                    if(m_itStack.empty())
                    {
                        m_first.clear();
                        m_end = true;
                        return *this;
                    }
                    m_itStack[m_itStack.size()-1].first++;
                    if(m_itStack[m_itStack.size()-1].first != m_itStack[m_itStack.size()-1].second)
                    {
                        m_first[m_first.size()-1] = m_itStack[m_itStack.size()-1].first->second->m_key;
                        break;
                    }
                }
            }
            else
            {
                m_first[m_first.size()-1] = m_itStack[m_itStack.size()-1].first->second->m_key;
            }

            while(! m_itStack[m_itStack.size()-1].first->second->final())
            {
                std::pair<decltype(m_ptr->m_children.begin()), decltype(m_ptr->m_children.end())> newLevel(m_itStack[m_itStack.size()-1].first->second->m_children.begin(),
                                                                                                           m_itStack[m_itStack.size()-1].first->second->m_children.end());
                m_itStack.push_back(newLevel);
                m_first.push_back(m_itStack[m_itStack.size()-1].first->second->m_key);
                if(m_itStack.size() == MAX_DEPTH)
                {
                    throw ETrieStackOverflow();
                }
            }

            return *this;
        }

        /**
         * @brief	operator==
         * @param[in] any end iterator
         * @returns true if at the end position
         */
        bool operator==(const TrieEndIterator& i_rhs) const NOEXCEPT
        {
            return m_end;
        }

        /**
         * @brief	operator!=
         */
		bool operator!=(const TrieEndIterator& i_rhs) const NOEXCEPT
        {
            return !(operator==(i_rhs));
        }

        /**
         * @brief	operator->
         * @returns pair of key and data at the node iterator is pointing now
         */
        std::pair<Key, Data&>* operator->()
        {
            if(m_pair)
            {
                delete m_pair;
            }

            m_pair = new std::pair<Key, Data&>(m_first, m_itStack[m_itStack.size()-1].first->second->data());
            return m_pair;
        }

    private:
        std::shared_ptr<TrieNode> m_ptr;
        bool m_end;
        Key m_first;
        std::pair<Key, Data&> *m_pair;
        using child_it = typename TrieNode::child_it;
        std::vector<std::pair<child_it, child_it>> m_itStack;
    };

    typedef TrieIterator iterator;
    typedef TrieEndIterator end_iterator;

	/**
     * @brief	Constructor of trie
	 */
    TrieDictionary(const Data& i_defaultValue)
    {
        m_defaultValue = i_defaultValue;
        m_top = std::make_shared<TrieNode>();
        m_size = 0;
    }

    /**
     * @brief	Destructor of trie
     */
    virtual ~TrieDictionary(){}

    /**
     * @brief	Checks if the object with the specified key exists in the trie
     * @param i_key - key
     * @returns true if there is a node with the key
    */
	bool has(const Key& i_key) NOEXCEPT
    {
        std::shared_ptr<TrieNode> current = m_top;
        size_t len = i_key.size();
        size_t depth = 0;
        for(auto& el : i_key)
        {
           if(current->hasChild(el))
           {
               current = current->getChild(el);
           }
           else
           {
               return false;
           }
           depth++;
           if(depth == len)
           {
               if(current->final())
               {
                   m_cacheKey = i_key;
                   m_cacheNode = current;
                   return true;
               }
               return false;
           }
        }
        return false;
    }

    /**
     * @brief	operator[]
     * @param[in] i_key - key
     * @returns data at the node or inserts new node otherwise
    */
    Data& operator[](const Key& i_key)
    {
        if(i_key == m_cacheKey)
        {
            return m_cacheNode->data();
        }
        std::shared_ptr<TrieNode> current = m_top;
        size_t len = i_key.size();
        size_t depth = 0;
        for(auto& el : i_key)
        {
           if(current->hasChild(el))
           {
               current = current->getChild(el);
           }
           else
           {
               current->addChild(el, m_defaultValue);
               current = current->getChild(el);
           }

           depth++;
           if(depth == len)
           {
               if(current->final()) return current->data();
               current->setFinal(true);
               m_size++;
               return current->data();
           }
        }
        throw std::exception();
    }

    /**
     * @brief	Returns number of elements
     * @returns Number of final nodes in the trie
     */
	size_t size() const NOEXCEPT{ return m_size; }
    /**
     * @brief	Returns end iterator
     * @returns end iterator
     */
	end_iterator end() const NOEXCEPT{ return m_end; }
    /**
     * @brief	Returns begin iterator
     * @returns begin iterator
     */
    iterator begin() const NOEXCEPT
    {
        if(m_size == 0)
        {
            return TrieIterator();
        }
        return TrieIterator(m_top);
    }

private:
    Key m_cacheKey;
    std::shared_ptr<TrieNode> m_cacheNode;
    std::shared_ptr<TrieNode> m_top;
    Data m_defaultValue;
    end_iterator m_end;
    size_t m_size;
};

class ETrieStackOverflow : public std::exception
{
public:
    ETrieStackOverflow() : std::exception()
    {
        std::cout << "Trie maximum depth exceeded at " << __LINE__ << std::endl;
    }
};

template<typename String, int ORDER>
class NgramStorage
{
public:
    NgramStorage() : m_ngramTrie(0), m_intKey(ORDER) {}
    int& operator[](const std::vector<String>& i_ngram)
    {
        int order = i_ngram.size() < ORDER ? i_ngram.size() : ORDER;
        for(int i = 0; i < order; i++)
        {
            if(m_wordVocab.find(i_ngram[i]) == m_wordVocab.end())
            {
                m_wordVocab[i_ngram[i]] = m_wordVocab.size();
            }
            m_intKey[i] = m_wordVocab[i_ngram[i]];
        }
        return m_ngramTrie[m_intKey];
    }
    bool has(const std::vector<String>& i_ngram)
    {
        int order = i_ngram.size() < ORDER ? i_ngram.size() : ORDER;
        for(int i = 0; i < order; i++)
        {
            if(m_wordVocab.find(i_ngram[i]) == m_wordVocab.end())
            {
                return false;
            }
            m_intKey[i] = m_wordVocab[i_ngram[i]];
        }
        return m_ngramTrie.has(m_intKey);
    }

private:
    TrieDictionary<std::vector<int>, int> m_ngramTrie;
    std::unordered_map<String, int> m_wordVocab;
    std::vector<int> m_intKey;
};

}

#endif
