#include "Routing.h"

bool Routing::testBuffer(Buffer* buffer) {
    if (buffer->link_used || buffer->r < MESSAGE_LENGTH)
        return false;
    return true;
}

void Routing::takeBuffer(Buffer* buffer) {
    assert(!buffer->link_used);
    assert(buffer->r >= MESSAGE_LENGTH);
    buffer->link_used = true;
    buffer->r -= MESSAGE_LENGTH;
}

vector<NodeInfo> Routing::forward(Message &s) {
    vector<NodeInfo> ret = {};
    vector<NodeInfo> infos= s.rpath[0];
    for (int i=0; i<infos.size(); ++i) {
//        vector<CubeNode *> temp_dsts;
//        for (int k=0; k<infos[i].dsts.size(); ++k){
//            temp_dsts.push_back((*cube)[infos[i].dsts[k]]);
//        }
//        (*cube)[infos[i].cur] -> temp_dsts;
        vector<NodeInfo> temp_infos = forwardOne(s, infos[i]);
        for (int k=0; k<temp_infos.size(); ++k) {
            ret.push_back(temp_infos[k]);
        }
    }
    return ret;
}

vector<NodeInfo> Routing::forwardOne(Message &s, NodeInfo cur_info) {
    vector<NodeInfo> ret;
    if (cur_info.dsts.empty()) {
        ret.push_back(cur_info);
        return ret;
    }

    unordered_map<int, vector<int>> delivery_record;
    //  key表示递送的维度，-1表示暂时不递送，-2表示确定无法递送；value表示该维度上的目标节点
    for (int i=-2; i<n; ++i) {
        delivery_record.insert(make_pair(i, vector<int>{}));
    }

    int dst_num = cur_info.dsts.size();
    string cur_id = int_to_binary_str(cur_info.cur, n);
    vector<string> dst_ids;
    for (int i=0; i<dst_num; ++i) {
        dst_ids.push_back(int_to_binary_str(cur_info.dsts[i], n));
    }

    vector<bool> buffer_can_use;
    for (int i=0; i<n; ++i) {
        Buffer* temp_buffer = (*cube)[cur_id]->link_buffers[i];
        buffer_can_use.push_back(testBuffer(temp_buffer));
    }

    for (int i=0; i<dst_num; ++i) {
        string dst_id = dst_ids[i];
        int pos = 0;
        while (pos < n) {
            if (cur_id[pos] != dst_id[pos])
                break;
            ++pos;
        }
        if (buffer_can_use[pos]) {
            (delivery_record.find(pos)->second).push_back(binary_str_to_int(dst_id));
        } else {
            (delivery_record.find(-1)->second).push_back(binary_str_to_int(dst_id));
        }
    }
    for (int i=0; i<n; ++i) {
        if (!buffer_can_use[i])
            continue;
        vector<int> temp_dst_digit_ids = delivery_record[i];
        if (temp_dst_digit_ids.empty())
            continue;
        string temp_cur_id = convert_01_by_pos(cur_id, i);
        int temp_cur_digit_id = binary_str_to_int(temp_cur_id);
        for (auto it = temp_dst_digit_ids.begin(); it < temp_dst_digit_ids.end(); ++it) {
            if (*it == temp_cur_digit_id) {
                temp_dst_digit_ids.erase(it);
                break;
            }
        }
        Buffer *temp_buffer = (*cube)[temp_cur_digit_id]->buffers[i];
        takeBuffer(temp_buffer);
        ret.push_back(NodeInfo(temp_cur_digit_id, temp_buffer,temp_dst_digit_ids));
    }
    if (!delivery_record[-1].empty())
        ret.push_back(NodeInfo(cur_info.cur, cur_info.buffer, delivery_record[-1]));

    return ret;
}
