#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Base.hpp"
#include <boost/shared_ptr.hpp>
#include <map>
//#include <vector>

namespace zpr
{
	/**
	 * Class representing unordered graph to hold streets information
	 */
	class Graph
	{
		friend class View;

		struct Vertex;
		//struct Edge;

		public:
		typedef boost::shared_ptr<Vertex> PVertex;
		//typedef boost::shared_ptr<Edge> PEdge;
		typedef std::map<int, PVertex> MVertices;

		private:
		/** Graph vertices */
		MVertices vertices_;

		public:
		/**
		 * Graph c-tor
		 */
		Graph();
		virtual ~Graph();

		/**
		 * Adds vertex into graph
		 *
		 * @param id vertex id
		 * @param position vertex position
		 */
		void addVertex(const int id, const Point position);
		
		/**
		 * Adds edge into graph between given vertices
		 *
		 * @param from starting vertex
		 * @param to ending vertex
		 */
		void addEdge(const int from, const int to);

		private:
		/**
		 * Vertex struct
		 */
		struct Vertex
		{
			/** Vertex id */
			const int id_;
			/** Vertex positoin */
			const Point position_;
			/** Outgoing vertices (to) */
			MVertices edges_;

			/**
			 * Vertex constructor
			 *
			 * @param id vertex id
			 * @param position vertex position
			 */
			Vertex(const int id, const Point position);
			virtual ~Vertex();
			
			/**
			 * Adds edge into vertex
			 *
			 * @param PVertex ending vertex
			 */
			void addEdge(const PVertex);
		};

		//struct Edge
		//{
		//	const PVertex from_;
		//	const PVertex to_;
		//	
		//	Edge(const PVertex, const PVertex);
		//	virtual ~Edge();

		//	bool operator==(const Edge&);
		//};
	};

	
}

#endif // GRAPH_HPP