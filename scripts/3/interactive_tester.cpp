#include <bits/stdc++.h>
using namespace std;

vector<bool> nodes(12, 0);
int currentNodes = 4;
int activeNodes = 4;
map<string, map<int, vector<int>>> locs;

vector<string> loadFileLines(const string& filePath) {
    vector<string> lines;
    ifstream file(filePath);
    string line;
    
    while (getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        lines.push_back(line);
    }
    
    return lines;
}

bool checker(vector<string> &inputLines, vector<string> &outputLines) {
    int m = 0;
    for (auto inp : inputLines) {
        istringstream iss(inp);
        vector<string> tokens((istream_iterator<string>(iss)), istream_iterator<string>());

        if (tokens.empty()) continue;

        string command = tokens[0];

        if (command == "upload") {
            string fileName = tokens[1];
            
            string filePath = "testcases/" + fileName;

            ifstream inFile(filePath, ifstream::binary | ifstream::ate);
            int fileSize = inFile.tellg();

            if(m >= outputLines.size())
            {
                return false;
            }

            int val = stoi(outputLines[m]);

            if(!locs[fileName].empty())
            {
                if(val == -1)
                {
                    m++;
                    continue;
                }
                return false;
            }

            int res = 1;
            if(activeNodes > 1)
            {
                res = 1;
            }
            else
            {
                res = -1;
            }

            if(res != val) return false;

            m++;

            if(res == 1)
            {
                int numChunks = (fileSize / 32) + (fileSize % 32 ? 1 : 0);
                for (int i = 0; i < numChunks; i++) {
                    if (m >= outputLines.size()) {
                        return false;
                    }

                    istringstream chunkStream(outputLines[m]);
                    vector<int> chunkData((istream_iterator<int>(chunkStream)), istream_iterator<int>());
                    
                    if (chunkData.size() < 2) {
                        return false;
                    }

                    int chunkNumber = chunkData[0];
                    int numLocations = chunkData[1];

                    if(chunkNumber != i)
                    {
                        return false;
                    }

                    if(numLocations < min(3, activeNodes - 1))
                    {
                        return false;
                    }

                    if (chunkData.size() != 2 + numLocations) {
                        return false;
                    }


                    for(int j = 0; j < numLocations; j++)
                    {
                        if(chunkData[2 + j] < 1 || chunkData[2 + j] >= currentNodes || !nodes[chunkData[2 + j]])
                        {
                            return false;
                        }
                        locs[fileName][chunkNumber].push_back(chunkData[2 + j]);
                    }

                    m++;
                }
            }
        } 
        else if (command == "retrieve") {
            if (m >= outputLines.size()) {
                return false;
            }
            string fileName = tokens[1];
            
            string filePath = "testcases/" + fileName;

            if(locs[fileName].empty())
            {
                if(stoi(outputLines[m]) == -1)
                {
                    m++;
                    continue;
                }
                return false;
            }

            int res = 1;

            ifstream inFile(filePath, ifstream::binary | ifstream::ate);
            int fileSize = inFile.tellg();

            int numChunks = (fileSize / 32) + (fileSize % 32 ? 1 : 0);

            for(int i = 0; i < numChunks; i++)
            {
                bool ok = false;
                for(auto nd: locs[fileName][i])
                {
                    if(nodes[nd]) ok = true;
                }

                if(!ok)
                {
                    res = -1;
                    break;
                }
            }

            if(res == -1)
            {
                int val = stoi(outputLines[m]);
                m++;
                if(val != -1) return false;
            }
            else
            {
                inFile.clear();
                inFile.seekg(0, ios::beg);

                string fileLine;
                while (getline(inFile, fileLine)) {
                    fileLine.erase(0, fileLine.find_first_not_of(" \t\r\n"));
                    fileLine.erase(fileLine.find_last_not_of(" \t\r\n") + 1);

                    if (m >= outputLines.size()) {
                        return false;
                    }

                    string outputLine = outputLines[m];
                    outputLine.erase(0, outputLine.find_first_not_of(" \t\r\n"));
                    outputLine.erase(outputLine.find_last_not_of(" \t\r\n") + 1);

                    if (fileLine != outputLine) {
                        return false;
                    }

                    m++;
                }
            }
        } 
        else if (command == "search") {
            if (m >= outputLines.size()) {
                return false;
            }

            string fileName = tokens[1];
            string word = tokens[2];

            if(locs[fileName].empty())
            {
                if(stoi(outputLines[m]) == -1)
                {
                    m++;
                    continue;
                }
                return false;
            }

            string filePath = "testcases/" + fileName;

            int res = 1;

            ifstream inFile(filePath, ifstream::binary | ifstream::ate);
            int fileSize = inFile.tellg();

            int numChunks = (fileSize / 32) + (fileSize % 32 ? 1 : 0);

            for(int i = 0; i < numChunks; i++)
            {
                bool ok = false;
                for(auto nd: locs[fileName][i])
                {
                    if(nodes[nd]) ok = true;
                }

                if(!ok)
                {
                    res = -1;
                    break;
                }
            }

            if(res == -1)
            {
                int val = stoi(outputLines[m]);
                m++;
                if(val != -1) return false;
            }
            else
            {
                inFile.clear();
                inFile.seekg(0, ios::beg);

                string fileContent((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
                inFile.close();

                vector<int> offsets;

                regex wordRegex("\\b" + word + "\\b");  // Match whole word only
                auto wordsBegin = sregex_iterator(fileContent.begin(), fileContent.end(), wordRegex);
                auto wordsEnd = sregex_iterator();

                for(auto it = wordsBegin; it != wordsEnd; ++it) {
                    offsets.push_back(static_cast<int>(it->position()));
                }


                int num = stoi(outputLines[m]);
                m++;
                
                if(num != offsets.size()) return false;

                if (m >= outputLines.size()) {
                    return false;
                }

                istringstream offsetStream(outputLines[m]);
                vector<int> outputOffsets((istream_iterator<int>(offsetStream)), istream_iterator<int>());

                sort(outputOffsets.begin(), outputOffsets.end());

                if(offsets != outputOffsets) return false;

                m++;
            }
        } 
        else if (command == "list_file") {
            string fileName = tokens[1];

            string filePath = "testcases/" + fileName;

            if(locs[fileName].empty())
            {
                if(stoi(outputLines[m]) == -1)
                {
                    m++;
                    continue;
                }
                return false;
            }

            ifstream inFile(filePath, ifstream::binary | ifstream::ate);
            int fileSize = inFile.tellg();
            inFile.close();

            int numChunks = (fileSize / 32) + (fileSize % 32 ? 1 : 0);

            for (int i = 0; i < numChunks; i++) {
                if (m >= outputLines.size()) {
                    return false;
                }

                istringstream chunkStream(outputLines[m]);
                vector<int> chunkData((istream_iterator<int>(chunkStream)), istream_iterator<int>());
                
                if (chunkData.size() < 2) {
                    return false;
                }

                int chunkNumber = chunkData[0];
                int numLocations = chunkData[1];

                if(chunkNumber != i)
                {
                    return false;
                }

                if (chunkData.size() != 2 + numLocations) {
                    return false;
                }

                vector<int> actual;
                vector<int> cur;
                for(int j = 0; j < numLocations; j++)
                {
                    if(chunkData[2 + j] < 1 || chunkData[2 + j] >= currentNodes || !nodes[chunkData[2 + j]])
                    {
                        return false;
                    }
                    cur.push_back(chunkData[2 + j]);
                }

                for(int j = 0; j < locs[fileName][i].size(); j++)
                {
                    if(nodes[locs[fileName][i][j]])
                    {
                        actual.push_back(locs[fileName][i][j]);
                    }
                }

                sort(cur.begin(), cur.end());
                sort(actual.begin(), actual.end());

                if(cur != actual)
                {
                    return false;
                }

                m++;
            }        
        } 
        else if (command == "failover") {
            if (m >= outputLines.size()) {
                return false;
            }

            int rank = stoi(tokens[1]);

            int val = stoi(outputLines[m]);

            if(val != 1) return false;

            nodes[rank] = 0;
            activeNodes--;

            m++;
        } 
        else if (command == "recover") {
            if (m >= outputLines.size()) {
                return false;
            }

            int rank = stoi(tokens[1]);

            int val = stoi(outputLines[m]);

            if(val != 1) return false;

            nodes[rank] = 1;
            activeNodes++;

            m++;
        } 
        else if (command == "exit") {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    string inputFilePath = argv[1];
    string outputFilePath = argv[2];
    currentNodes = std::stoi(argv[3]);;
    activeNodes = currentNodes;

    for(int i = 1; i < currentNodes; i++)
    {
        nodes[i] = 1;
    }

    vector<string> inputLines = loadFileLines(inputFilePath);
    vector<string> outputLines = loadFileLines(outputFilePath);

    bool ok = checker(inputLines, outputLines);

    if(!ok) return 1;

    return 0;
}