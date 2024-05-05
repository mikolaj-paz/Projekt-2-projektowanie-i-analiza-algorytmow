#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <vector>
#include <chrono>
#include <random>
#include <limits.h>
#include <array>
#include <fstream>
#include <string>

#include "graph.hpp"
#include "dijkstra_algorithm.hpp"

#ifndef TIME_POINT
#define TIME_POINT std::chrono::high_resolution_clock::now()
#endif

class DataManager
{
    public:
        DataManager() = delete;

        template <typename C>
        static void printContainer(const C& container)
        {
            std::cout << "{ ";
            for (auto & i : container)
                std::cout << i << ' ';
            std::cout << '}' << std::endl;
        }

        template <typename C>
        static void printPointerContainer(const C& container)
        {
            std::cout << "{ ";
            for (auto & i : container)
                std::cout << *i << ' ';
            std::cout << '}' << std::endl;
        }

        static void saveToCSV(const std::string filePath, double** data, const int& rowNum, const int& colNum, const char& separator)
        {
            std::ofstream output(filePath);
            // if (!output.is_open()) throw Exception("File couldn't open in saveToCSV()");

            // for (int i = 0; i < header.size() - 1; i++)
            //     output << header[i] << separator;
            // output << header[header.size() - 1] << '\n';

            for (int j = 0; j < rowNum; j++)
            {
                for (int i = 0; i < colNum - 1; i++)
                    output << data[j][i] << separator;
                output << data[j][colNum - 1] << '\n';
            }
            output << std::flush;

            output.close();
        }

        static double** createTimesTable(GraphADT<int,int>* graph, const int N[], const int& sizeN, const double D[], const int& sizeD)
        {
            double** out = new double*[sizeD + 1];
            
            out[0] = new double[sizeN + 1];
            out[0][0] = 0;
            for (int i = 0; i < sizeN; i++)
                out[0][i + 1] = static_cast<double>(N[i]);

            for (int d = 0; d < sizeD; d++)
            {
                out[d + 1] = new double[sizeN + 1];
                out[d + 1][0] = D[d];
                for (int n = 0; n < sizeN; n++)
                {
                    for (int k = 1; k <= 20; k++)
                    {
                        graph->clear();
                        auto ranStart = TIME_POINT;
                            DataManager::createRandomSimpleUndirectedGraph(graph, N[n], D[d]);
                        auto ranEnd = TIME_POINT;
                        std::cout << k << ". n=" << N[n] << " d=" << D[d]
                                  << "\n    Rand     = " << std::chrono::duration(ranEnd - ranStart).count() / 1E6 
                                  << " ms\n";

                        auto vs = graph->vertices();

                        auto start = TIME_POINT;
                            DijkstraAlgorithm::shortestPath(graph, vs[DataManager::getRandomInt(0, vs.size() - 1)]);
                        auto end = TIME_POINT;

                        auto diff = std::chrono::duration(end - start).count() / 1E6;
                        std::cout << "    Dijkstra = " << diff << " ms" << std::endl;
                        out[d + 1][n + 1] += diff;
                    }
                    out[d + 1][n + 1] /= 20;
                    std::cout << "==================================\n"
                              << "n=" << N[n] << " d=" << D[d]
                              << "\navg = " << out[d + 1][n + 1] << " ms\n"
                              << "==================================\n";
                }
                
            }

            return out;
        }

        static void createRandomSimpleUndirectedGraph(GraphADT<int,int>* graph, const int& verticesNumber, const double& density)
        {
            Vertex<int,int>* vertices[verticesNumber];
            int incidentEdgesNum[verticesNumber];
            for (int i = 0; i < verticesNumber; i++)
                incidentEdgesNum[i] = 0;

            for (int i = 0; i < verticesNumber; i++)
                vertices[i] = (graph->insertVertex(i + 1));

            const int targetEdgesNumber = density * verticesNumber * (verticesNumber - 1) / 2;

            for (int i = 0; i < targetEdgesNumber; i++)
            {
                int v;
                do
                    v = getRandomInt(0, verticesNumber - 1);
                while (incidentEdgesNum[v] == verticesNumber - 1);
                int w;
                do
                    w = getRandomInt(0, verticesNumber - 1);
                while (w == v || graph->areAdjacent(vertices[v], vertices[w]));
                graph->insertEdge(vertices[v], vertices[w], getRandomInt(1, INT_MAX - 1));
                ++incidentEdgesNum[v];
                ++incidentEdgesNum[w];
            }
        }

        static int getRandomInt(const int& min, const int& max)
        {
            auto seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::mt19937 generator(seed);
            std::uniform_int_distribution<int> distribution(min, max);
            return distribution(generator);
        }
};

#endif