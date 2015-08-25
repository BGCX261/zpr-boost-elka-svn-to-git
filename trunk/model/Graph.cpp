#include "Graph.hpp"

namespace zpr
{
	/************
	*	Graph	*
	************/
	Graph::Graph() {}

	Graph::~Graph() {}

	void Graph::addVertex(const int id, const Point position)
	{
		vertices_.insert( std::pair<int, PVertex>( id, PVertex( new Vertex(id, position) ) ) );
	}

	void Graph::addEdge(const int from, const int to)
	{
		if ( vertices_.find(from) != vertices_.end() && vertices_.find(to) != vertices_.end() )
		{
			vertices_[from]->addEdge( vertices_[to] );
			//vertices_[to]->addEdge( vertices_[from] );
		}
	}
	
	/************
	*	Vertex	*
	************/
	Graph::Vertex::Vertex(const int id, const Point position): id_(id), position_(position)
	{
	}

	Graph::Vertex::~Vertex() {}

	void Graph::Vertex::addEdge(const Graph::PVertex vertex)
	{
		edges_.insert( std::pair<int, PVertex>(vertex->id_, vertex) );
	}

	/************
	*	Edge	*
	************/
	//Graph::Edge::Edge(const Graph::PVertex from, const Graph::PVertex to): from_(from), to_(to)
	//{
	//}

	//Graph::Edge::~Edge() {}
}