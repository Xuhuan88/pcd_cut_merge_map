#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

import rosbag
import sys
import math

# 检查输入参数
if len(sys.argv) < 4:
    print "Usage: python cut_bag.py input.bag output_prefix num_splits"
    sys.exit(1)

input_bagfile = sys.argv[1]
output_prefix = sys.argv[2]
num_splits = int(sys.argv[3])

# 打开输入bag文件
input_bag = rosbag.Bag(input_bagfile, 'r')

# 获取输入bag文件的起始时间和持续时间
start_time = input_bag.get_start_time()
end_time = input_bag.get_end_time()
duration = end_time - start_time

# 计算每个输出bag文件的时间范围
split_duration = int(math.ceil(duration / num_splits))

# 初始化变量
current_bag_index = 1
current_bag_start_time = start_time
current_bag_end_time = current_bag_start_time + split_duration
output_bag = None

# 遍历输入bag文件中的消息
for topic, msg, t in input_bag.read_messages():
    # 检查当前消息的时间是否在当前输出bag文件的时间范围内
    if t.to_sec() >= current_bag_start_time and t.to_sec() <= current_bag_end_time:
        # 创建新的输出bag文件
        if output_bag is None:
            output_bag = rosbag.Bag(output_prefix + str(current_bag_index) + '.bag', 'w')

        # 写入消息到当前bag文件
        output_bag.write(topic, msg, t)

    # 如果当前消息的时间大于等于当前输出bag文件的结束时间，则关闭当前bag文件并递增索引
    if t.to_sec() >= current_bag_end_time:
        output_bag.close()
        current_bag_index += 1
        current_bag_start_time = current_bag_end_time
        current_bag_end_time = current_bag_start_time + split_duration
        output_bag = None

    # 如果已经达到指定的输出文件数量，则跳出循环
    if current_bag_index > num_splits:
        break

# 关闭输入bag文件
input_bag.close()

print "Bag splitting complete."
