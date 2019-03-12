#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <vector>

class GraphNode
{
private:
	char			   m_Identifier;
	std::vector<GraphNode*> m_pChildNodes;
	bool			    m_bVisited;

public:
	GraphNode() : m_pChildNodes(0), m_Identifier(0), m_bVisited(0) { ; }
	GraphNode(int id) : m_pChildNodes(0), m_Identifier(id), m_bVisited(0) { ; }
	~GraphNode();

	char GetID() const { return m_Identifier; }
	std::vector<GraphNode*> GetChildNodes()	const { return m_pChildNodes; }
	void AddAChildNode(GraphNode* pNewNode) { m_pChildNodes.push_back(pNewNode); }

	void SetVisited(bool yesNo) { m_bVisited = yesNo; }
	bool GetVisited() { return m_bVisited; }
};

#endif //GraphNode_h