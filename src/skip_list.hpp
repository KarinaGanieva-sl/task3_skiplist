////////////////////////////////////////////////////////////////////////////////
// Module Name:  skip_list.h/hpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

// !!! DO NOT include skip_list.h here, 'cause it leads to circular refs. !!!

#include <cstdlib>
#include "skip_list.h"


//==============================================================================
// class NodeSkipList
//==============================================================================

template <class Value, class Key, int numLevels>
void NodeSkipList<Value, Key, numLevels>::clear(void)
{
    for (int i = 0; i < numLevels; ++i)
        Base::nextJump[i] = 0;

    Base::levelHighest = -1;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(void)
{
    clear();
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey)
{
    clear();

    Base::Base::key = tkey;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey, const Value& val)
{
    clear();

    Base::Base::key = tkey;
    Base::Base::value = val;
}

//==============================================================================
// class SkipList
//==============================================================================

template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::SkipList(double probability)
{
    _probability = probability;

    // Lets use m_pPreHead as a final sentinel element
    for (int i = 0; i < numLevels; ++i)
        Base::_preHead->nextJump[i] = Base::_preHead;

    Base::_preHead->levelHighest = numLevels - 1;
}

template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::~SkipList()
{

}


    // TODO: !!! One need to implement all declared methods !!!

template <class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::insert(const Value& val, const Key& key)
{

   Node* nodesForUpdate[numLevels];
   Node* tmp = Base::_preHead;
   for(int i = numLevels - 1; i >= 0; i--)
   {
       while(tmp->nextJump[i] != Base::_preHead && tmp->nextJump[i]->key <= key)
           tmp = tmp->nextJump[i];
       nodesForUpdate[i] = tmp;
   }
   int newLevel = randLevel();
   Node* newNode = new Node(key, val);
   newNode->levelHighest = newLevel;
   newNode->next = nodesForUpdate[0]->nextJump[0];
   for(int i = 0; i <= newLevel; i++)
   {
       newNode->nextJump[i] = nodesForUpdate[i]->nextJump[i];
       nodesForUpdate[i]->nextJump[i] = newNode;
   }
    nodesForUpdate[0]->next =newNode;
}

template <class Value, class Key, int numLevels>
int SkipList<Value, Key, numLevels>::randLevel()
{
    int level = 0;
    while(((double) rand() / (RAND_MAX)) < _probability && level < numLevels - 1)
        level++;
    return level;
}
template <class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::removeNext(Node* nodeBefore)
{
    if(!nodeBefore)
        throw std::invalid_argument("null");
    Key keyForRemoving = nodeBefore->next->key;
    Node* nodesForUpdate[numLevels];
    Node* tmp = Base::_preHead;
    for(int i = numLevels - 1; i >= 0; i--)
    {
        while(tmp->nextJump[i] != Base::_preHead && tmp->nextJump[i]->key < keyForRemoving)
            tmp = tmp->nextJump[i];
        nodesForUpdate[i] = tmp;
    }
    Node* prev = tmp;
    tmp = tmp->next;
    if(tmp == Base::_preHead || tmp->key != keyForRemoving)
        throw std::invalid_argument("wrong");
    for(int i = 0; i < numLevels; i++)
    {
        if(nodesForUpdate[i]->nextJump[i] != tmp)
            break;
        nodesForUpdate[i]->nextJump[i] = tmp->nextJump[i];
    }
    prev->next = tmp->next;
    delete tmp;
}

template <class Value, class Key, int numLevels>
typename SkipList<Value,Key,numLevels>::Node* SkipList<Value, Key, numLevels>::findLastLessThan(const Key& key) const
{
    Node* tmp = Base::_preHead;
    for(int i = numLevels - 1; i >= 0; i--)
    {
        while(tmp->nextJump[i] != Base::_preHead && tmp->nextJump[i]->key < key)
            tmp = tmp->nextJump[i];
    }
    if(tmp->next->key < key)
        tmp = tmp->next;
    return tmp;
}

template<class Value, class Key, int numLevels>
typename SkipList<Value,Key,numLevels>::Node *SkipList<Value, Key, numLevels>::findFirst(const Key &key) const
{
    Node* tmp = findLastLessThan(key);
    std::cout<<"  "<<tmp->key;
    if(tmp == tmp->next || tmp->next == Base::_preHead)
        return nullptr;
    std::cout<<"ppp";
    if(tmp->next->key != key)
        return nullptr;
    return tmp->next;
}




