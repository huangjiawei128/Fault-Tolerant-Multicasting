#include "Cube.h"

Cube::Cube(int n, int buffer_size) {
    this->n = n;
    this->buffer_size = buffer_size;
    node_num = pow(2, n);
    head = new CubeNode[node_num];
    assert(head);

    for (int i = 0; i < node_num; ++i) {
        (head + i)->Initialize(this, i);
        (head + i)->setBuffer(buffer_size);
    }

    for (int i = 0; i < node_num; ++i) {
        (head + i)->setLinkBuffer();
    }
}

void Cube::setFault(string id) {
    (*this)[id]->fault = true;
}

void Cube::setFault(int digit_id) {
    (*this)[digit_id]->fault = true;
}

bool Cube::isFaulty(string id) {
    return (*this)[id]->fault;
}

bool Cube::isFaulty(int digit_id) {
    return (*this)[digit_id]->fault;
}


CubeNode *Cube::operator[](string id) {
    return head + binary_str_to_int(id);
}

CubeNode *Cube::operator[](int digit_id) {
    return head + digit_id;
}

void Cube::setMscs() {
    //  BFS
    queue<string> qu;
    qu.push(string(n, '*'));
    unordered_set<string> not_mscs = {};

    while (!qu.empty()) {
        string cur_subcube = qu.front();
        qu.pop();
        if (not_mscs.find(cur_subcube) != not_mscs.end()) {
            continue;
        }
        pair<SubcubeState, NodeStateMap> cur_state_info = getSubcubeStateInfo(cur_subcube);
        SubcubeState cur_state = cur_state_info.first;
        NodeStateMap cur_node_state_map = cur_state_info.second;
        vector<string> cur_subcube_children = getSubcubeChildren(cur_subcube);
        if (cur_state == SAFE) {
            msc_info.insert(make_pair(cur_subcube, cur_node_state_map));
            for (auto it = cur_subcube_children.begin(); it != cur_subcube_children.end(); ++it) {
                not_mscs.insert(*it);
            }
        } else {
            for (auto it = cur_subcube_children.begin(); it != cur_subcube_children.end(); ++it) {
                qu.push(*it);
            }
        }
    }

//    cout << "msc: " << endl;
//    for (auto it=msc_info.begin(); it!=msc_info.end(); ++it) {
//        cout << it->first << endl;
//    }
//    cout << endl << endl;

    return;
}

pair<SubcubeState, NodeStateMap> Cube::getSubcubeStateInfo(string subcube) {
    assert(subcube.size() == n);
    vector<string> node_ids = getNodesInSubcube(subcube);
    int node_num = node_ids.size();
    NodeStateMap node_state_map;
    for (int i = 0; i < node_num; ++i) {
        string node_id = node_ids[i];
        if (isFaulty(node_id)) {
            node_state_map.insert(make_pair(node_id, FAULTY));
        } else {
            node_state_map.insert(make_pair(node_id, L_SAFE));
        }
    }

    bool flag = true;
    while (flag) {
        flag = false;   //  若有节点发生状态修改，将flag置为true
        for (int i = 0; i < node_num; ++i) {
            string node_id = node_ids[i];
            NodeState old_state = node_state_map[node_id];
            if (old_state == FAULTY) {
                continue;
            }
            vector<string> neigh_ids = getNodeNeighsInSubcube(node_id, subcube);
            vector<NodeState> neigh_states;
            int neigh_num = neigh_ids.size();
            for (int j = 0; j < neigh_num; ++j) {
                string neigh_id = neigh_ids[j];
                neigh_states.push_back(node_state_map[neigh_id]);
            }
            NodeState new_state = judgeNodeStateByNeighStates(neigh_states);
            if (new_state != old_state) {
                node_state_map[node_id] = new_state;
                flag = true;
            }
        }
    }

    SubcubeState subcube_state = FULLY_UNSAFE;
    for (auto it = node_state_map.begin(); it != node_state_map.end(); ++it) {
        if (it->second == L_SAFE) {
            subcube_state = SAFE;
            break;
        }
    }

    return make_pair(subcube_state, node_state_map);
}

vector<string> Cube::getSubcubeChildren(string subcube) {
    vector<string> ret;
    vector<int> unfixed_posis;
    for (int i = 0; i < subcube.size(); ++i) {
        if (subcube[i] == '*') {
            unfixed_posis.push_back(i);
        }
    }
    int count = unfixed_posis.size();
    for (int i = 0; i < count; ++i) {
        int posi = unfixed_posis[i];
        string child_subcube = subcube;
        child_subcube[posi] = '0';
        ret.push_back(child_subcube);
        child_subcube[posi] = '1';
        ret.push_back(child_subcube);
    }

    return ret;
}

bool Cube::isNodeInSubcube(string node_id, string subcube) {
    assert(node_id.size() == n);
    assert(subcube.size() == n);
    for (int i = 0; i < n; ++i) {
        if (subcube[i] != '*' && node_id[i] != subcube[i]) {
            return false;
        }
    }
    return true;
}

vector<string> Cube::getNodesInSubcube(string subcube) {
    assert(subcube.size() == n);
    vector<string> ret;
    vector<int> unfixed_posis;
    for (int i = 0; i < subcube.size(); ++i) {
        if (subcube[i] == '*') {
            unfixed_posis.push_back(i);
        }
    }
    int count = unfixed_posis.size();
    for (int i = 0; i < pow(2, count); ++i) {
        string node_id = subcube;
        string s = int_to_binary_str(i, count);
        for (int j = 0; j < count; ++j) {
            int posi = unfixed_posis[j];
            node_id[posi] = s[j];
        }
        ret.push_back(node_id);
    }
    return ret;
}

vector<string> Cube::getNodeNeighsInSubcube(string node_id, string subcube) {
    if (!isNodeInSubcube(node_id, subcube)) {
        return {};
    }

    vector<string> ret;
    vector<int> unfixed_posis;
    for (int i = 0; i < n; ++i) {
        if (subcube[i] == '*') {
            unfixed_posis.push_back(i);
        }
    }
    int count = unfixed_posis.size();
    for (int i = 0; i < count; ++i) {
        string neigh_id = node_id;
        int posi = unfixed_posis[i];
        neigh_id[posi] = convert_01(neigh_id[posi]);
        ret.push_back(neigh_id);
    }
    return ret;
}

NodeState Cube::judgeNodeStateByNeighStates(vector<NodeState> neigh_states) {
    NodeState ret;
    int faulty_num = 0;
    int l_ordinarily_unsafe_num = 0;
    int l_strongly_unsafe_num = 0;
    int l_safe_num = 0;
    int neigh_state_num = neigh_states.size();
    for (int i = 0; i < neigh_state_num; ++i) {
        if (neigh_states[i] == FAULTY)
            ++faulty_num;
        else if (neigh_states[i] == L_ORDINARILY_UNSAFE)
            ++l_ordinarily_unsafe_num;
        else if (neigh_states[i] == L_STRONGLY_UNSAFE)
            ++l_strongly_unsafe_num;
        else if (neigh_states[i] == L_SAFE)
            ++l_safe_num;
    }
    int l_unsafe_num = l_ordinarily_unsafe_num + l_strongly_unsafe_num;
    if (faulty_num >= 2 || (l_unsafe_num + faulty_num) >= 3) {
        if (l_safe_num >= 1)
            ret = L_ORDINARILY_UNSAFE;
        else
            ret = L_STRONGLY_UNSAFE;
    } else {
        ret = L_SAFE;
    }
    return ret;
}

int Cube::getLocalSafetyValueInMsc(string node_id, string msc) {
    assert(msc_info.find(msc) != msc_info.end());
    int ret = 0;
    vector<string> neigh_ids = getNodeNeighsInSubcube(node_id, msc);
    int neigh_num = neigh_ids.size();
    for (int i = 0; i < neigh_num; ++i) {
        string neigh_id = neigh_ids[i];
        if (msc_info[msc][neigh_id] == L_SAFE) {
            ++ret;
        }
    }
    return ret;
}

unordered_map<string, NodeStateMap> Cube::getSameMsc(string node_id1, string node_id2) {
    unordered_map<string, NodeStateMap> ret;
    for (auto it = msc_info.begin(); it != msc_info.end(); ++it) {
        string msc = it->first;
        if (isNodeInSubcube(node_id1, msc) && isNodeInSubcube(node_id2, msc)) {
            ret.insert(*it);
        }
    }
    return ret;
}

int Cube::getDistanceBetweenNode(string node_id1, string node_id2) {
    int ret = 0;
    for (int i = 0; i < n; ++i) {
        if (node_id1[i] != node_id2[i]) {
            ++ret;
        }
    }
    return ret;
}

vector<string> Cube::getSrcNodeNeighsInMinPath(string src_id, string dst_id) {
    vector<string> ret;
    vector<int> diff_posis;
    for (int i = 0; i < n; ++i) {
        if (src_id[i] != dst_id[i]) {
            diff_posis.push_back(i);
        }
    }
    int count = diff_posis.size();
    for (int i = 0; i < count; ++i) {
        string neigh_id = src_id;
        int posi = diff_posis[i];
        neigh_id[posi] = convert_01(neigh_id[posi]);
        ret.push_back(neigh_id);
    }
    return ret;
}

vector<string> Cube::getSrcNodeNeighsOutMinPath(string src_id, string dst_id) {
    vector<string> ret;
    vector<int> same_posis;
    for (int i = 0; i < n; ++i) {
        if (src_id[i] == dst_id[i]) {
            same_posis.push_back(i);
        }
    }
    int count = same_posis.size();
    for (int i = 0; i < count; ++i) {
        string neigh_id = src_id;
        int posi = same_posis[i];
        neigh_id[posi] = convert_01(neigh_id[posi]);
        ret.push_back(neigh_id);
    }
    return ret;
}

int Cube::getFirstDiffDirection(string node_id1, string node_id2) {
    int ret = -1;
    for (int i=0; i<n; ++i) {
        if (node_id1[i] != node_id2[i]) {
            ret = i;
            break;
        }
    }
    return ret;
}
