/**
----------------------------------------------------------------------------
@file    tidl_graph_model.c
@brief     Visualisation utility for tidl_models (converts to dot
format (.gv) for graphviz)
----------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>
//#include <cstdlib>
#include <itidl_ti.h>
#include<cgraph.h>
#include<gvc.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#define MAX_LAYER_NODES 2048
#define MAX_LAYER_EDGES 2048
#define DEBUG_PRINT 0

#include "tidl_runtimes_import_common.h"

const char *TIDL_graphBgColors[] =
{
  "#375588",    //"Convolution",
  "#338533",    //"Pooling",
  "#4B1616",    //"ReLU",
  "#209070",    //"EltWise",
  "#30C0D0",    //"InnerProduct",
  "#B07320",    //"BatchNorm",
  "#DC143C",    //"Bias",
  "#802C4C",    //"Deconv2D",
  "#804000",    //"Concat",
  "#9932CC",    //"Split",
  "#6495ED",    //"Slice",
  "#B0C4DE",    //"Crop",
  "#778899",    //"Flatten",
  "#87CEEB",    //"DropOut",
  "#007F9F",    //"ArgMax",
  "#5499C7",    //"DetectionOutput",
  "#30C0B0",    //"ShuffleChannel" ,
  "#A84060",    //"Resize" ,
  "#BB8FCE",    //"RoiPooling",
  "#3366CC",    //"OdPostProcessing",
  "#EE82EE",    //"Scale"
};

void * TIDL_readAllowlistNodesFile(char * fname, std::vector<std::vector<int>>& supportedNodeGroups)
{
  std::ifstream fAllowlistNode(fname);

  int32_t supportedNodeGroupsSize;

  if(fAllowlistNode.is_open())
  {
    fAllowlistNode >> supportedNodeGroupsSize;
    
    for(int i = 0; i < supportedNodeGroupsSize; i++)
    {
      std::vector<int> subgraph;
      int32_t subgraphSize;
      int32_t nodeIdx;

      fAllowlistNode >> subgraphSize;
      
      for(int j = 0; j < subgraphSize; j++)
      {
        fAllowlistNode >> nodeIdx;
        subgraph.push_back(nodeIdx);
      }
      supportedNodeGroups.push_back(subgraph);
    }
    fAllowlistNode.close();
  }
  else
  {
    printf("Warning : Unable to open %s , graph visualisation may be incorrect \n", fname);
  }
  return 0;
}

int32_t TIDL_readGraphvizInfoFile(char * fname, sTIDL_runtimesVisualisationInfo_t ** info)
{
  std::ifstream fNodeInfo(fname);

  int32_t graphSize;
  std::string line;
  if(fNodeInfo.is_open())
  {
    std::getline(fNodeInfo, line);
    std::stringstream ss;
    ss << line;
    ss >> graphSize;
  }
  else
  {
    printf("Warning : Unable to open %s , graph visualisation may be incorrect \n", fname);
    return 0;
  }
  sTIDL_runtimesVisualisationInfo_t * visInfo = new sTIDL_runtimesVisualisationInfo_t[graphSize];
  (*info) = visInfo;
  
  for(int i = 0; i < graphSize; i++)
  {
    std::getline(fNodeInfo, line);
    if(line.length() != 0)
    {
      std::stringstream linestream(line);
      linestream >> visInfo[i].nodeModelIdx;
      linestream >> visInfo[i].nodeName;
      linestream >> visInfo[i].opType;

      int adjNode = -1;
      std::string adjType;
      int32_t numAdjNodes;
      int32_t numAddOutput;
      linestream >> adjType;
      if(adjType.compare("outputAdjNodes") == 0)
      {
        linestream >> numAdjNodes;
        if(numAdjNodes == 0)
        {
          linestream >> visInfo[i].graphOutputNames;
        }
        for(int j = 0; j < numAdjNodes; j++)
        {
          linestream >> adjNode;
          visInfo[i].outputAdjNodes.push_back(adjNode);
        }
      }
      linestream >> adjType;
      if(adjType.compare("inputAdjNodes") == 0)
      {
        linestream >> numAdjNodes;
        if(numAdjNodes == 0)
        {
          linestream >> visInfo[i].graphInputNames;
        }
        for(int j = 0; j < numAdjNodes; j++)
        {
          linestream >> adjNode;
          visInfo[i].inputAdjNodes.push_back(adjNode);
        }
      }
      linestream >> adjType;
      if(adjType.compare("additionalOutput") == 0)
      {
        linestream >> numAddOutput;
        if(numAddOutput != 0)
        {
          linestream >> visInfo[i].graphOutputNames;
        }
      }
      std::string diagName;
      std::string diagInfoStr;
      linestream >> diagName;
      std::getline(linestream, diagInfoStr); //extracts till it hits "\n"
      visInfo[i].diagnosticInfo = diagInfoStr;
    }
  }
#if DEBUG_PRINT
  //print file data
  for(int i = 0; i < graphSize; i++)
  {
    std::cout << visInfo[i].nodeModelIdx << " " << visInfo[i].nodeName << " " << visInfo[i].opType << " ";
    for(auto &x : visInfo[i].outputAdjNodes)
    {
      std::cout << x << " ";
    }
    std::cout << visInfo[i].graphInputNames << " " << visInfo[i].graphOutputNames << " ";
    std::cout << visInfo[i].diagnosticInfo;
    std::cout << std::endl;
  }
#endif
  return graphSize;
}

void TIDL_checkEdgeRobustness(int32_t graphSize, sTIDL_runtimesVisualisationInfo_t * visInfo)
{
  for(int i = 0; i < graphSize; i++)
  {
    for(auto& j : visInfo[i].outputAdjNodes)
    {
      if(std::find(visInfo[j].inputAdjNodes.begin(), visInfo[j].inputAdjNodes.end(), i) == visInfo[j].inputAdjNodes.end())
      {
        printf("Robustness check failed in graph visualization : %d not an input of %d , edges may not be correct in visualization \n", i, j);
      }
    }
  }
}

void TIDL_addTIDLSubgraphsToGraph(Agraph_t *g, Agraph_t * subg, Agnode_t * (&n)[MAX_LAYER_NODES], Agsym_t * sym, int32_t& nodeIdx, int32_t& subgIdx, 
                                  std::vector<std::vector<int>> supportedNodeGroups, sTIDL_runtimesVisualisationInfo_t *visInfo, std::vector<bool>& visitedNodes, 
                                  std::map<int, int>& modelToGraphIdxMapping)
{
  for(int i = 0; i < supportedNodeGroups.size(); i++)
	{
		std::string clusterName = "cluster_" + std::to_string(subgIdx);
    
    //create subgraph
    char *cstr = new char[clusterName.length() + 1];
    strcpy(cstr, clusterName.c_str());
    
    subg = agsubg(g, cstr, 1);  //subgraph created
    
    delete [] cstr;

    //set properties for subgraph box
    sym = agattr(subg, AGRAPH, "color", (char*)TIDL_graphBgColors[i]);
    sym = agattr(subg, AGRAPH, "style", "rounded");
    sym = agattr(subg, AGRAPH, "penwidth", "2");
    
    subgIdx++;

    std::vector<int> subgraph = supportedNodeGroups[i];
    for(int j = 0; j < subgraph.size(); j++)
    {
      std::string nodeInfo = "Name : " + visInfo[subgraph[j]].nodeName;
      if((visInfo[subgraph[j]].diagnosticInfo.compare("") != 0) || (visInfo[subgraph[j]].diagnosticInfo.compare(" ") != 0) || (visInfo[subgraph[j]].diagnosticInfo.compare("\n") != 0))
      {
        nodeInfo = nodeInfo + "\nInfo :" + visInfo[subgraph[j]].diagnosticInfo;
      }
      
      //create node
      char *cstr = new char[nodeInfo.length() + 1];
      strcpy(cstr, nodeInfo.c_str());
      
      n[nodeIdx] = agnode(subg, cstr, TRUE);   // create graph node for TIDL subgraph node
      agset(n[nodeIdx], "fillcolor", (char*)TIDL_graphBgColors[i]); //one color for all nodes of a given subgraph
      
      delete [] cstr;
      
      //label node
      std::string nodeLabel = std::to_string(subgraph[j]) + ":" + visInfo[subgraph[j]].opType;
      cstr = new char[nodeLabel.length() + 1];
      strcpy(cstr, nodeLabel.c_str());
      
      agset(n[nodeIdx], "label", cstr); //node label

      delete [] cstr;

      modelToGraphIdxMapping.insert(std::pair<int, int>(subgraph[j], nodeIdx));
      
      nodeIdx++;
      visitedNodes[subgraph[j]] = true;
    }
	}
}

void TIDL_addARMNodestoGraph(Agraph_t *g, Agnode_t * (&n)[MAX_LAYER_NODES], int32_t& nodeIdx, int32_t graphSize, sTIDL_runtimesVisualisationInfo_t *visInfo, 
                             std::vector<bool>& visitedNodes, std::map<int, int>& modelToGraphIdxMapping)
{
  for(int i = 0; i < graphSize; i++)
  {
    if(! visitedNodes[i])
    {
      std::string nodeLabel = std::to_string(i) + ":" + visInfo[i].opType;
      char *nodeLabelChar = new char[nodeLabel.length() + 1];
      strcpy(nodeLabelChar, nodeLabel.c_str());
      
      std::string nodeInfo = "Name : " + visInfo[i].nodeName + "\nInfo :" + visInfo[i].diagnosticInfo;
      char *nodeInfoChar = new char[nodeInfo.length() + 1];
      strcpy(nodeInfoChar, nodeInfo.c_str());

      n[nodeIdx] = agnode(g, (char*)nodeInfoChar, TRUE);
      agset(n[nodeIdx], "label", (char*)nodeLabelChar);
      agset(n[nodeIdx], "fillcolor", (char*)"#707070");   //Gray color for ARM nodes
      modelToGraphIdxMapping.insert(std::pair<int, int>(i, nodeIdx));
      delete [] nodeLabelChar;
      delete[] nodeInfoChar;
      nodeIdx++;
      visitedNodes[i] = true;
    }
  }
}

void TIDL_addEdgesToGraph(Agraph_t *g, Agnode_t * (&n)[MAX_LAYER_NODES], Agedge_t * (&e)[MAX_LAYER_EDGES], int32_t graphSize,
                          sTIDL_runtimesVisualisationInfo_t *visInfo, std::map<int, int>& modelToGraphIdxMapping)
{
  for(int i = 0; i < graphSize; i++)
  {
    for(int j = 0; j < visInfo[i].outputAdjNodes.size(); j++)
    {
      std::string edgeName = "Edge for layer " + std::to_string(i) + "--" + std::to_string(j);
      char * edgeNameChar = new char[edgeName.length() + 1];
      strcpy(edgeNameChar, edgeName.c_str());
      e[i] = agedge(g, n[modelToGraphIdxMapping.at(i)], n[modelToGraphIdxMapping.at(visInfo[i].outputAdjNodes[j])], (char *)edgeNameChar, TRUE);
      delete [] edgeNameChar;
    }
  }
}

void TIDL_addGraphInputs(Agraph_t *g, Agnode_t * (&n)[MAX_LAYER_NODES], Agedge_t * (&e)[MAX_LAYER_EDGES], int32_t graphSize, int32_t& nodeIdx,
                          sTIDL_runtimesVisualisationInfo_t *visInfo, std::map<int, int>& modelToGraphIdxMapping)
{
  int32_t graphInputNodeIdx[graphSize] = {-1};
  bool isInputAdded = false;

  for(int i = 0; i < graphSize; i++)
  {
    isInputAdded = false;
    if(! visInfo[i].graphInputNames.empty()) // This node is connected to graph input
    { 
      std::string inputName = "Input : " + std::to_string(i);
      char *id = new char[inputName.length() + 1];
      strcpy(id, inputName.c_str());
      for(int j = 0; j < i; j++)
      {
        if(visInfo[j].graphInputNames.compare(visInfo[i].graphInputNames) == 0) //input already added
        {
          e[i] = agedge(g, n[graphInputNodeIdx[j]], n[modelToGraphIdxMapping.at(i)], (char *)id, TRUE);
          isInputAdded = true;
          break;
        }
      }
      if(! isInputAdded)
      {
        n[nodeIdx] = agnode(g, (char*)id, TRUE);
        graphInputNodeIdx[i] = nodeIdx;
        agset(n[nodeIdx], "label", (char*)"Input");
        e[i] = agedge(g, n[nodeIdx], n[modelToGraphIdxMapping.at(i)], (char *)id, TRUE);
        nodeIdx++;
      }
      delete [] id;
    }
  }
}

void TIDL_addGraphOutputs(Agraph_t *g, Agnode_t * (&n)[MAX_LAYER_NODES], Agedge_t * (&e)[MAX_LAYER_EDGES], int32_t graphSize, int32_t& nodeIdx,
                          sTIDL_runtimesVisualisationInfo_t *visInfo, std::map<int, int>& modelToGraphIdxMapping)
{
  int32_t graphOutputNodeIdx[graphSize] = {-1};
  bool isOutputAdded = false;
  for(int i = 0; i < graphSize; i++)
  {
    isOutputAdded = false;
    if(! visInfo[i].graphOutputNames.empty()) // This node is connected to graph output
    { 
      std::string outputName = "Output : " + std::to_string(i);
      char *id = new char[outputName.length() + 1];
      strcpy(id, outputName.c_str());
      for(int j = 0; j < i; j++)
      {
        if(visInfo[j].graphOutputNames.compare(visInfo[i].graphOutputNames) == 0) //output already added
        {
          e[i] = agedge(g, n[modelToGraphIdxMapping.at(i)], n[graphOutputNodeIdx[j]], (char *)id, TRUE);
          isOutputAdded = true;
          break;
        }
      }
      if(! isOutputAdded)
      {
        n[nodeIdx] = agnode(g, (char*)id, TRUE);
        graphOutputNodeIdx[i] = nodeIdx;
        agset(n[nodeIdx], "label", (char*)"Output");
        e[i] = agedge(g, n[modelToGraphIdxMapping.at(i)], n[nodeIdx], (char *)id, TRUE);
        nodeIdx++;
      }
      delete [] id;
    }
  }
}

void TIDL_createVisualizationGraph(char * foutputFilePath, int32_t graphSize, std::vector<std::vector<int>> supportedNodeGroups, sTIDL_runtimesVisualisationInfo_t * visInfo)
{
  Agnode_t *n[MAX_LAYER_NODES];
  Agedge_t *e[MAX_LAYER_EDGES];
  Agraph_t *g;
  GVC_t *gvc;
  gvc = gvContext();

  g = agopen(foutputFilePath, Agdirected, NULL);
  
  Agraph_t *subg;

  //graph parameters
  Agsym_t *sym;
  sym = agattr(g, AGNODE,"color", "#000000");
  sym = agattr(g,AGNODE, "fontcolor", "#FFFFFF");
  sym = agattr(g,AGNODE, "fontsize", "10");
  sym = agattr(g,AGNODE, "height", "0.12");
  sym = agattr(g, AGNODE, "style", "rounded,filled");
  sym = agattr(g, AGNODE, "penwidth", "1");
  sym = agattr(g, AGNODE, "fillcolor", "#000000");
  sym = agattr(g, AGRAPH, "pad", "6.5,0.2,6.5,0.5");
  sym = agattr(g, AGRAPH, "rankdir", "TB");
  sym = agattr(g, AGRAPH, "ranksep","0.1");
  sym = agattr(g,AGEDGE, "fontsize", "8");
  sym = agattr(g, AGEDGE, "penwidth", "0.7");
  sym = agattr(g, AGEDGE, "arrowhead", "vee");
  sym = agattr(g, AGEDGE, "arrowsize", "0.5");
  sym = agattr(g, AGNODE, "shape", "box");

	std::vector<bool> visitedNodes;
	visitedNodes.assign(graphSize ,false);
  
  int32_t subgIdx = 0;
  int32_t nodeIdx = 0;
  std::map<int, int> modelToGraphIdxMapping;
  
  //Add TIDL delegated subgraphs to graph
  TIDL_addTIDLSubgraphsToGraph(g, subg, n, sym, nodeIdx, subgIdx, supportedNodeGroups, visInfo, visitedNodes, modelToGraphIdxMapping);
  
  //Add ARM delegated nodes to graph
  TIDL_addARMNodestoGraph(g, n, nodeIdx, graphSize, visInfo, visitedNodes, modelToGraphIdxMapping);
  
  //Add edges
  TIDL_addEdgesToGraph(g, n, e, graphSize, visInfo, modelToGraphIdxMapping);

  //Add inputs
  TIDL_addGraphInputs(g, n, e, graphSize, nodeIdx, visInfo, modelToGraphIdxMapping);
  
  //Add outputs
  TIDL_addGraphOutputs(g, n, e, graphSize, nodeIdx, visInfo, modelToGraphIdxMapping);

  gvLayout(gvc, g, "dot");
  gvRenderFilename(gvc, g, "svg", foutputFilePath);
  gvFreeLayout(gvc, g);

  agclose(g);
}

int main(int argc,char *argv[])
{
  if(argc < 4)
  {
    printf("Enter allowedNode.txt, graphvizInfo.txt and output file path ! Exiting !\n");
    exit(-1);
  }
  char * fnameAllowlistNode = argv[1];
  char * fnameNodesInfo = argv[2];
  char * foutputFilePath = argv[3];

  std::vector<std::vector<int>> supportedNodeGroups;
  TIDL_readAllowlistNodesFile(fnameAllowlistNode, supportedNodeGroups);
  
  sTIDL_runtimesVisualisationInfo_t * visInfo;
  
  int32_t graphSize = TIDL_readGraphvizInfoFile(fnameNodesInfo, &visInfo);

  TIDL_checkEdgeRobustness(graphSize, visInfo);

  TIDL_createVisualizationGraph(foutputFilePath, graphSize, supportedNodeGroups, visInfo);

  delete [] visInfo;

  return 0;
}
