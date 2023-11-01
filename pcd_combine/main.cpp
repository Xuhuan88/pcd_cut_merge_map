// 把第 n 个点云地图和第 n-1 个点云地图进行拼接

#include <iostream>
#include <Eigen/Dense>
#include "Eigen/Core"
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/common/transforms.h>
#include <vector>

#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

struct PointCloudInfo {
    std::string filePath;
    double x;
    double y;
    double z;
    double degree_x;
    double degree_y;
    double degree_z;
    std::vector<double> pose;
};


//根据欧拉角计算旋转矩阵
Eigen::Matrix3d rotation(std::vector<double> pose){
    Eigen::Matrix3d R;
    R = Eigen::AngleAxisd(pose[5], Eigen::Vector3d::UnitZ()) *
           Eigen::AngleAxisd(pose[4], Eigen::Vector3d::UnitY()) *
           Eigen::AngleAxisd(pose[3], Eigen::Vector3d::UnitX());
    return R;
}


int main() {
    // 读取map_info.yaml文件
    YAML::Node mapInfo = YAML::LoadFile("../map_info.yaml");
    int n = mapInfo["point_clouds"].size();
    // 存储读取的PCD文件
    std::vector<std::string> filePaths;
    std::vector<std::vector<double>> poses;

    std::cout << "[ log ] number of pcd to combine: " << n << std::endl;
    // 提取点云文件路径、XYZ参数和角度
    for (const auto& pointCloud : mapInfo["point_clouds"]) {
        PointCloudInfo info;
        info.filePath = pointCloud["filePath"].as<std::string>();
        info.x = pointCloud["x"].as<double>();;
        info.y = pointCloud["y"].as<double>();;
        info.z = pointCloud["z"].as<double>();;
        info.degree_x = pointCloud["degree_x"].as<double>();;
        info.degree_y = pointCloud["degree_y"].as<double>();;
        info.degree_z = pointCloud["degree_z"].as<double>();;
        info.pose = {info.x, info.y, info.z,
                               info.degree_x, info.degree_y, info.degree_z};


        filePaths.push_back(info.filePath);
        poses.push_back(info.pose);
    }

    std::string output_file;
    std::cout << "请输入输出pcd文件名：";
    std::cin >> output_file;


    // 合并所有点云
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_all(new pcl::PointCloud<pcl::PointXYZI>);
    // 加载最后一张点云地图
pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_last(new pcl::PointCloud<pcl::PointXYZI>);
if (pcl::io::loadPCDFile<pcl::PointXYZI>(filePaths[n-1], *cloud_last) == -1) {
    std::cerr << "无法加载文件: " << filePaths[n-1] << std::endl;
    return -1;
}

*cloud_all += *cloud_last;  // 将最后一张点云地图添加到合并的点云地图中

Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();

for (int i = n - 2; i >= 0; --i) {
    // 根据欧拉角（roll, pitch, yaw）计算旋转矩阵
    std::cout << "i: " << i << std::endl;
    std::cout << "pose[i]: ";
    for (const auto& value : poses[i]) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    Eigen::Matrix3d R = rotation(poses[i]);
    Eigen::Vector3d T(poses[i][0], poses[i][1], poses[i][2]);

    transform.block<3, 3>(0, 0) = R;
    transform.block<3, 1>(0, 3) = T;

    // 加载第 i 个点云地图
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_i(new pcl::PointCloud<pcl::PointXYZI>);
    if (pcl::io::loadPCDFile<pcl::PointXYZI>(filePaths[i] , *cloud_i) == -1) {
        std::cerr << "无法加载文件: " << filePaths[i]  << std::endl;
        return -1;
    }

    // 转换点云
    pcl::PointCloud<pcl::PointXYZI>::Ptr transformed_cloud(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::transformPointCloud(*cloud_all, *transformed_cloud, transform);

    // 合并点云
    *cloud_all =*cloud_i + *transformed_cloud;
}

   

    // 保存为二进制格式的pcd点云文件
    pcl::io::savePCDFileBinary(output_file, *cloud_all);

    std::cout << "拼接完成，结果保存为: " << output_file << std::endl;



    

    return 0;
}

// 1. 先读取所有的点云文件


