import pandas as pd
import matplotlib.pyplot as plt

fault_nodes_num = 5

src_dir = './result'
dst_dir = './figure'

src_path = src_dir + '/fault_nodes_num=%s.csv' % str(fault_nodes_num)
df = pd.read_csv(src_path)
link_rate = df['link_rate'].tolist()
throughput = df['throughput'].tolist()
latency = df['latency'].tolist()
fail_percent = df['fail_percent'].tolist()

plt.plot(link_rate, throughput)
plt.xlabel('link rate (message/node/cycle)')
plt.ylabel('throughput (message/node/cycle)')
dst_path = dst_dir + '/throughput_fault_nodes_num=%s.svg' % str(fault_nodes_num)
plt.savefig(dst_path, dpi=600)
plt.close()

plt.plot(link_rate, latency)
plt.xlabel('link rate (message/node/cycle)')
plt.ylabel('latency (cycle)')
dst_path = dst_dir + '/latency_fault_nodes_num=%s.svg' % str(fault_nodes_num)
plt.savefig(dst_path, dpi=600)
plt.close()

plt.plot(link_rate, fail_percent)
plt.xlabel('link rate (message/node/cycle)')
plt.ylabel('fail percent')
dst_path = dst_dir + '/fail_percent_fault_nodes_num=%s.svg' % str(fault_nodes_num)
plt.savefig(dst_path, dpi=600)
plt.close()
