template <class T>
void DialogGraph<T>::AddNode(const T& value)
{
	nodes.Add(Node(value));
	++count;
}

template <class T>
void DialogGraph<T>::AddNode(T&& value)
{
	nodes.Add(Node(MoveTemp(value)));
	++count;
}

template <class T>
template <class ... Args>
void DialogGraph<T>::EmplaceNode(Args... args)
{
	nodes.Emplace(args);
	++count;
}

template <class T>
void DialogGraph<T>::AddEdge(int startID, int endID)
{
	ensure(startID > 0 && startID < Count());
	ensure(endID > 0 && endID < Count());

	nodes[startID]->AddEdge(startID, endID);
	++count;
}

template <class T>
void DialogGraph<T>::Node::AddEdge(int startID, int endID)
{
	edges.Add(Edge(startID, endID));
}