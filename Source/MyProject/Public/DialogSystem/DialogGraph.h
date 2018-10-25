// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include <iterator>

/**
 * Basic graph class and iterator
 */

class DialogGraphIterator
{
 public:
   typedef std::input_iterator_tag iterator_category; // readonly iterator
   typedef int                     value_type;
   typedef std::size_t             difference_type;
   typedef int*                    pointer;
   typedef int&                    reference;

   DialogGraphIterator(int value) : current(value) {}

   DialogGraphIterator& operator++()
   {
      ++current;
      return *this;
   }
   DialogGraphIterator operator++(int) { return DialogGraphIterator(current++); }

   value_type operator*() const { return current; }

   bool operator==(DialogGraphIterator const& rhs) { return current == rhs.current; }
   bool operator!=(DialogGraphIterator const& rhs) { return current != rhs.current; }

 private:
   int current;
};

template <class T>
class MYPROJECT_API DialogGraph
{
 public:
   DialogGraph(){};
   ~DialogGraph(){};

   void AddNode(const T& value);
   void AddNode(T&& value);
   void AddEdge(int startID, int endID); // contract, stay within ID boundaries
   template <class... Args>
   void EmplaceNode(Args... args);

   int                 Count() const { return count; }
   DialogGraphIterator begin() const { return DialogGraphIterator(0); }
   DialogGraphIterator end() const { return DialogGraphIterator(nodes.Num()); }

 private:
   class Node;

   TArray<Node*> nodes;
   int           count = 0;

   class Edge
   {
    public:
      Edge(int startID, int destID) : startID(startID), destID(destID) {}

    private:
      int startID;
      int destID;
   };

   class Node
   {
    public:
      Node(T data) : data(data) {}
      T const& GetNodeData() const { return data; }

    private:
      T            data;
      TArray<Edge> edges;

      void        AddEdge(int startID, int endID);
      friend void DialogGraph::AddEdge(int, int);
   };
};

#include "DialogGraph.tpp"
