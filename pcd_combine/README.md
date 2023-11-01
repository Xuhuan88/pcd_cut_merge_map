# 点云地图合并功能包
#@ creator: hoozhawed
#@ date: 2023.10.29
#@ version: 1.0.0
#@ email: hoozhawed@gmail.com

## 功能介绍
- 点云地图合并功能包

## 功能描述
- 在bag文件中，放置需要拆分的ros数据包；
- 运行cut.py，即可将bag文件中的数据包拆分成多个小ros数据包；
- 将ros数据包各自建图，并记录下autoware-ndt_mapping运行时，终端最后输出的三维旋转关系；
- 在map文件夹中，放入需要合并的点云地图文件；
- 在map_info.yaml文件中，配置需要合并的点云地图文件路径及三维旋转关系；
- 运行程序文件，即可将map文件夹中的所有点云地图文件合并成一个点云地图文件；

## 功能实现
- 1.拆包
- $ python3 cut_bag_py3.py input.bag output.bag bag_num
- 视python版本使用相应脚本
- 2.通过autoware将拆分的bag包出成pcd地图，记录文件路径、XYZ 坐标和欧拉角，按map_info.yaml格式写入
- 3.编译pcd地图拼接代码并运行脚本 
- $ mkdir budild
- $ cd build
- $ cmake ..
- $ make
- $ ./pcd_merge

## 参数配置
- 参数1：点云地图文件路径
- 参数2：各个小地图对应的三维旋转关系；
