// Created 5/8/21 6:43 PM

#include "UpTreeStructs.h"

FUpTreeNode* FUpTreeNode::NextLeft()
{
   if(Children.Num())
   {
      return Children[0];
   }
   else
   {
      return Thread;
   }
}

FUpTreeNode* FUpTreeNode::NextRight()
{
   if(Children.Num())
   {
      return Children[Children.Num() - 1];
   }
   else
   {
      return Thread;
   }
}

FUpTreeNode* FUpTreeNode::GetLeftSibling() const
{
   if(Parent && ChildIndex)
   {
      return Parent->Children[ChildIndex - 1];
   }
   return nullptr;
}

FUpTreeConnector& FUpTreeConnector::operator=(const FUpTreeConnector& other)
{
   if(this == &other) return *this;
   Control1 = other.Control1;
   Control2 = other.Control2;
   Control3 = other.Control3;
   Control4 = other.Control4;
   return *this;
}

FUpTreeConnector& FUpTreeConnector::operator=(FUpTreeConnector&& other) noexcept
{
   if(this == &other) return *this;
   Control1 = MoveTemp(other.Control1);
   Control2 = MoveTemp(other.Control2);
   Control3 = MoveTemp(other.Control3);
   Control4 = MoveTemp(other.Control4);
   return *this;
}
