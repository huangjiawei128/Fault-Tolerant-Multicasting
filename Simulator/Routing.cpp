#include "Routing.h"

bool Routing::testBuffer(Buffer *buffer) {
    if (buffer->link_used || buffer->r < MESSAGE_LENGTH)
        return false;
    return true;
}

void Routing::takeBuffer(Buffer *buffer) {
    assert(!buffer->link_used);
    assert(buffer->r >= MESSAGE_LENGTH);
    buffer->link_used = true;
    buffer->bufferMin(MESSAGE_LENGTH);
}

vector<int> Routing::getPossibleDirection(int cur, int dst, unordered_set<int>& passed) {
    vector<int> ret = {};
    string cur_id = int_to_binary_str(cur, n);
    string dst_id = int_to_binary_str(dst, n);
    unordered_map<string, NodeStateMap> same_mscs =
            cube->getSameMsc(cur_id, dst_id);
    vector<string> neighs_in_min_path = cube->getSrcNodeNeighsInMinPath(cur_id, dst_id);
    vector<string> neighs_out_min_path = cube->getSrcNodeNeighsOutMinPath(cur_id, dst_id);

    if (same_mscs.empty()) {    //  目标节点u和当前节点v不包含在任何一个msc中
        vector<string> l_safe_neighs = {};
        vector<string> l_ordinarily_unsafe_neighs = {};
        vector<string> l_strongly_unsafe_neighs = {};
        for (auto it=neighs_in_min_path.begin(); it!=neighs_in_min_path.end(); ++it) {
            unordered_map<string, NodeStateMap> _same_mscs =
                    cube->getSameMsc(*it, dst_id);
            if (_same_mscs.empty()) {
                continue;
            }
            NodeState temp_neigh_state = _same_mscs.begin()->second[*it];
            switch (temp_neigh_state) {
                case L_SAFE:
                    l_safe_neighs.push_back(*it);
                    break;
                case L_ORDINARILY_UNSAFE:
                    l_ordinarily_unsafe_neighs.push_back(*it);
                    break;
                case L_STRONGLY_UNSAFE:
                    l_strongly_unsafe_neighs.push_back(*it);
                    break;
            }
        }

        //  Pri1：最小路径中的locally safe邻居v’，使得存在一个msc包含v’和u
        if (!l_safe_neighs.empty()) {
            for (auto it = l_safe_neighs.begin(); it!=l_safe_neighs.end(); ++it) {
                if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                    continue;
                }
                ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
            }
        }

        //  Pri2：最小路径中的locally ordinarily unsafe邻居v’，使得存在一个msc包含v’和u
        if (ret.empty()) {
            if (!l_ordinarily_unsafe_neighs.empty()) {
                for (auto it = l_ordinarily_unsafe_neighs.begin();
                    it!=l_ordinarily_unsafe_neighs.end(); ++it) {
                    if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                        continue;
                    }
                    ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                }
            }
        }

        //  Pri3：最小路径中的locally strongly unsafe邻居v’，使得存在一个msc包含v’和u
        if (ret.empty()) {
            if (!l_strongly_unsafe_neighs.empty()) {
                for (auto it = l_strongly_unsafe_neighs.begin();
                    it!=l_strongly_unsafe_neighs.end(); ++it) {
                    if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                        continue;
                    }
                    ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                }
            }
        }
    } else {    //  目标节点u和当前节点v包含在某一个msc中
        string msc = same_mscs.begin()->first;
        NodeStateMap node_state_map = same_mscs.begin()->second;
        NodeState cur_state = node_state_map[cur_id];
        assert(cur_state != FAULTY);
        int dis = cube->getDistanceBetweenNode(cur_id, dst_id);
        if (dis <= 2) { //  H(v,u) ≤ 2
            //  Pri1：最小路径中的fault-free邻居
            for (auto it=neighs_in_min_path.begin(); it!=neighs_in_min_path.end(); ++it) {
                if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                    continue;
                }
                NodeState temp_state = node_state_map[*it];
                if (temp_state != FAULTY) {
                    ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                }
            }

            //  Pri2：非最小路径中有最大local safety值的邻居
            if (ret.empty()) {
                int max_local_safety_value = -1;
                vector<string> possible_neigh_ids = {};
                for (auto it=neighs_out_min_path.begin(); it!=neighs_out_min_path.end(); ++it) {
                    if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                        continue;
                    }
                    NodeState temp_state = node_state_map[*it];
                    if (temp_state == FAULTY) {
                        continue;
                    }
                    int temp_local_safety_value = cube->getLocalSafetyValueInMsc(*it, msc);
                    if (temp_local_safety_value > max_local_safety_value) {
                        max_local_safety_value = temp_local_safety_value;
                        possible_neigh_ids.clear();
                        possible_neigh_ids.push_back(*it);
                    } else if (temp_local_safety_value == max_local_safety_value) {
                        possible_neigh_ids.push_back(*it);
                    }
                }

                for (auto it = possible_neigh_ids.begin(); it != possible_neigh_ids.end(); ++it) {
                    ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                }
            }
        } else {    //  H(v,u) ≥ 3
            if (cur_state == L_SAFE) {  //  v为locally safe
                //  Pri1：最小路径中的locally safe邻居
                for (auto it=neighs_in_min_path.begin(); it!=neighs_in_min_path.end(); ++it) {
                    if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                        continue;
                    }
                    NodeState temp_state = node_state_map[*it];
                    if (temp_state == L_SAFE) {
                        ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                    }
                }
            } else if (cur_state == L_ORDINARILY_UNSAFE) {  //  v为locally ordinarily unsafe
                //  Pri1：最小路径中的locally safe邻居
                for (auto it=neighs_in_min_path.begin(); it!=neighs_in_min_path.end(); ++it) {
                    if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                        continue;
                    }
                    NodeState temp_state = node_state_map[*it];
                    if (temp_state == L_SAFE) {
                        ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                    }
                }

                //  Pri2：最小路径中的locally ordinarily unsafe邻居
                if (ret.empty()) {
                    for (auto it=neighs_in_min_path.begin(); it!=neighs_in_min_path.end(); ++it) {
                        if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                            continue;
                        }
                        NodeState temp_state = node_state_map[*it];
                        if (temp_state == L_ORDINARILY_UNSAFE) {
                            ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                        }
                    }
                }

                //  Pri3：非最小路径中的locally safe邻居
                if (ret.empty()) {
                    for (auto it=neighs_out_min_path.begin(); it!=neighs_out_min_path.end(); ++it) {
                        if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                            continue;
                        }
                        NodeState temp_state = node_state_map[*it];
                        if (temp_state == L_SAFE) {
                            ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                        }
                    }
                }
            } else if (cur_state == L_STRONGLY_UNSAFE) {    //  v为locally strongly unsafe
                //  Pri1：最小路径中的locally ordinarily unsafe邻居
                for (auto it=neighs_in_min_path.begin(); it!=neighs_in_min_path.end(); ++it) {
                    if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                        continue;
                    }
                    NodeState temp_state = node_state_map[*it];
                    if (temp_state == L_ORDINARILY_UNSAFE) {
                        ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                    }
                }

                //  Pri2：最小路径中的locally strongly unsafe邻居
                if (ret.empty()) {
                    for (auto it=neighs_in_min_path.begin(); it!=neighs_in_min_path.end(); ++it) {
                        if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                            continue;
                        }
                        NodeState temp_state = node_state_map[*it];
                        if (temp_state == L_STRONGLY_UNSAFE) {
                            ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                        }
                    }
                }

                //  Pri3：非最小路径中的locally ordinarily unsafe邻居
                if (ret.empty()) {
                    for (auto it=neighs_out_min_path.begin(); it!=neighs_out_min_path.end(); ++it) {
                        if (passed.find(binary_str_to_int(*it)) != passed.end()) {
                            continue;
                        }
                        NodeState temp_state = node_state_map[*it];
                        if (temp_state == L_ORDINARILY_UNSAFE) {
                            ret.push_back(cube->getFirstDiffDirection(cur_id, *it));
                        }
                    }
                }
            }
        }
    }

    return ret;
}

vector<NodeInfo> Routing::forward(Message &s) {
    vector<NodeInfo> ret = {};
    vector<NodeInfo> infos = s.rpath[0];

    //  依次对首flit所在的每个当前节点计算下一跳节点列表
    for (int i = 0; i < infos.size(); ++i) {
        vector<NodeInfo> temp_infos = forwardOne(s, infos[i]);

//        for (auto it=temp_infos.begin(); it!=temp_infos.end(); ++it) {
//            assert(!(*cube)[it->cur]->fault);
//        }

        for (int k = 0; k < temp_infos.size(); ++k) {
            ret.push_back(temp_infos[k]);
        }
    }

    return ret;
}

vector<NodeInfo> Routing::forwardOne(Message &s, NodeInfo cur_info) {
    //  若目标节点为空，直接返回
    vector<NodeInfo> ret;
    if (cur_info.dsts.empty()) {
        ret.push_back(cur_info);
        return ret;
    }

    unordered_map<int, vector<int>> delivery_record;
    //  key表示递送的方向，-1表示暂时不递送，-2表示确定无法递送；value表示该方向上确定递送的目标节点
    for (int i = -2; i < n; ++i) {
        delivery_record.insert(make_pair(i, vector<int>{}));
    }
    unordered_map<int, vector<int>> direction_to_dsts;    //  key表示递送的方向，value表示该方向上可能递送的目标节点
    for (int i = 0; i < n; ++i) {
        direction_to_dsts.insert(make_pair(i, vector<int>{}));
    }
    vector<pair<int, pair<int, int>>> ras_records;
    //  key表示递送的方向，value表示<该方向的ras，对应缓存的剩余容量（不可用时置为-1）>

    vector<int> dsts = cur_info.dsts;
    string cur_id = int_to_binary_str(cur_info.cur, n);

    //  初始化buffer_can_use
    vector<bool> buffer_can_use;    //  表示当前节点各个方向上的link buffer是否可用
    for (int i = 0; i < n; ++i) {
        Buffer *temp_buffer = (*cube)[cur_id]->link_buffers[i];
        buffer_can_use.push_back(testBuffer(temp_buffer));
    }

    //  决策各个目标节点对应的可能递送方向，填好direction_to_dsts；如果确定无法递送则填入delivery_record[-2]
    for (auto dst_it = dsts.begin(); dst_it != dsts.end(); ++dst_it) {
        vector<int> possible_directions = getPossibleDirection(cur_info.cur, *dst_it, cur_info.passed);    
        //  每个目标节点对应的下一跳可能方向
        if (possible_directions.empty()) {
            (delivery_record.find(-2)->second).push_back(*dst_it);
        } else {
            for (auto direction_it = possible_directions.begin(); direction_it != possible_directions.end();
                 ++direction_it) {
                direction_to_dsts[*direction_it].push_back(*dst_it);
            }
        }
    }

    //  初始化ras_records，将其按ras从大到小、缓存剩余容量（不可用时置为-1）从大到小的优先级排序
    for (int i = 0; i < n; ++i) {
        int ras = direction_to_dsts[i].size();
        int buffer_cap = -1;
        if (buffer_can_use[i]) {
            buffer_cap = (*cube)[cur_id]->link_buffers[i]->r;
        }
        ras_records.push_back(make_pair(i, make_pair(ras, buffer_cap)));
    }
    sort(ras_records.begin(), ras_records.end(), [](pair<int, pair<int, int>> x, pair<int, pair<int, int>> y) {
        if (x.second.first == y.second.first) {
            return x.second.second > y.second.second;
        }
        return x.second.first > y.second.first;
    });

    //  根据ras_records决定各个目标节点最终的递送方向
    unordered_set<int> decided_dsts;
    for (int i = 0; i < n; ++i) {
        int direction = ras_records[i].first;
        int _direction = direction;
        if (buffer_can_use[direction] == false) {
            _direction = -1;
        }
        vector<int> direction_dsts = direction_to_dsts[direction];
        for (auto it = direction_dsts.begin(); it != direction_dsts.end(); ++it) {
            if (decided_dsts.find(*it) != decided_dsts.end())
                continue;
            (delivery_record.find(_direction)->second).push_back(*it);
            decided_dsts.insert(*it);
        }
    }

    //  根据delivery_record得到所有下一跳节点的NodeInfo
    for (int i = 0; i < n; ++i) {
        if (!buffer_can_use[i])
            continue;
        vector<int> temp_dsts = delivery_record[i];
        if (temp_dsts.empty())
            continue;
        string temp_cur_id = convert_01_by_pos(cur_id, i);
        int temp_cur = binary_str_to_int(temp_cur_id);
        for (auto it = temp_dsts.begin(); it < temp_dsts.end(); ++it) {
            if (*it == temp_cur) {
                temp_dsts.erase(it);
                break;
            }
        }
        Buffer *temp_buffer = (*cube)[temp_cur]->buffers[i];
        takeBuffer(temp_buffer);
        ret.push_back(NodeInfo(temp_cur, temp_buffer, temp_dsts, cur_info.passed));
    }
    if (!delivery_record[-1].empty()) {
        ret.push_back(NodeInfo(cur_info.cur, cur_info.buffer, delivery_record[-1], cur_info.passed));
    }
    if (!delivery_record[-2].empty()) {
        s.fault_delivery += delivery_record[-2].size();
        if (delivery_record[-1].empty()) {
            ret.push_back(NodeInfo(cur_info.cur, cur_info.buffer, {}, cur_info.passed));
        }
    }

    return ret;
}
