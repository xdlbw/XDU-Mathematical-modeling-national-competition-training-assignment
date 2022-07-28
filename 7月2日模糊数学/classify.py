import os
from PIL import Image
import numpy as np
from scipy import stats
from math import sqrt

# 对图片进行统一化处理（得到灰度矩阵、灰度直方统计）
def get_img_data(image_name):
    # 将图片转换为L模式，其为灰度图，其每个像素用8个bit表示
    image = Image.open(image_name)
    gray_image = image.convert('L')
    hm = np.zeros(256)
    pixels = gray_image.getdata()
    mm = np.mat(pixels, int).reshape(20, 30)  # 图片均为30*20像素
    for pixel in pixels:
        hm[pixel] += 1
    return mm, hm


# 得到平均值哈希
def get_ahash(mmatrix):
    ahash = ""
    ave = np.average(mmatrix)
    row, col = np.shape(mmatrix)
    # print(row, col)
    for i in range(row):
        for j in range(col):
            # print(mmatrix[i, j])
            if mmatrix[i, j] < ave:  # 灰度值小于平均值记为0
                ahash += '0'
            else:
                ahash += '1'
    # print(ahash)
    return ahash


# 得到整张图片的灰度加权算术平均值
def get_awei(hmatrix):
    awei = 0
    num = 0
    for i,v in enumerate(hmatrix):
        # print(i, v)
        awei += (i+1)*v
        num += 1
    awei = awei / num
    return awei


# 得到一张图片的LBP特征向量
def get_lbp_vec(mmatrix):
    # 得到一个像素点的lbp值
    def get_onepx_lbpv(x, y):
        lbp = ''
        center = mmatrix[x, y]
        # 顺时针比较、填充数据
        direction = [(-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1)]
        for d in direction:
            if mmatrix[x + d[0], y + d[1]] < center:
                lbp += '0'
            else:
                lbp += '1'
        return int(lbp, base=2)
    # 分成6块10*10
    lbp_vec = np.zeros(6)
    for i in range(2):
        for j in range(3):  # 每一块
            lbps = np.zeros(64, 'int')
            k = 0
            start_x = 1 + i*10
            start_y = 1 + j*10
            for x in range(start_x, start_x+8):  # 每一块里的像素点
                for y in range(start_y, start_y+8):
                    lbps[k] = get_onepx_lbpv(x, y)
                    k += 1
            mode = stats.mode(lbps)
            lbp_vec[j+i*3] = mode[0]  # lbps里的众数
    return lbp_vec


# 得到一张图片的特征描述字典（样本特征）
def get_character_dic(image):
    mm, hm = get_img_data(image)
    c_vec = {
        'ahash': (get_ahash(mm)),
        'awei': (get_awei(hm)),
        'lbp_vec': (get_lbp_vec(mm)),
    }
    return c_vec


# 计算两个平均值哈希的结果（汉明距离）
def cal_ahashs(ahash1, ahash2):
    n = 0
    # 遍历判断
    for i in range(len(ahash1)):
        # 不相等则n计数+1，n最终为相似度
        if ahash1[i] != ahash2[i]:
            n = n + 1
    return 1 - n/len(ahash1)


# 计算两个灰度加权平均值的结果（相对差距）
def cal_aweis(awei1, awei2):
    return 1 - abs((awei2-awei1))/256


#计算两个lbp向量的结果（夹角余弦法）
def cal_lbpvecs(lbpvec1, lbpvec2):
    n = len(lbpvec1)
    x1 = 0
    for i in range(n):
        x1 += lbpvec1[i]*lbpvec2[i]
    x2 = 0
    for i in range(n):
        x2 += lbpvec1[i] * lbpvec1[i]
    x3 = 0
    for i in range(n):
        x3 += lbpvec1[i] * lbpvec1[i]
    return x1/(sqrt(x2)*sqrt(x3))


# 计算两张图片的相似度（加权平均）
def get_two_imgs_similiarity(img1, img2, weight=[0.34, 0.61, 0.05]):
    c1 = get_character_dic(img1)
    c2 = get_character_dic(img2)
    simi = weight[0]*(cal_ahashs(c1['ahash'], c2['ahash'])) + weight[1]*(cal_aweis(c1['awei'], c2['awei'])) + weight[2] * (cal_lbpvecs(c1['lbp_vec'], c2['lbp_vec']))
    return simi


# 得到一组图片的关系矩阵R和闭包CR和聚类数量
def get_imgs_r_cr_n(imgs, lam=0.5):
    num = len(imgs)
    r = np.mat(np.zeros((num, num)), 'float')
    for i in range(num):
        for j in range(num):
            if i == j:
                r[i, j] = 1
            elif i > j:
                r[i, j] = r[j, i]  # 对称矩阵
            else:
                r[i, j] = get_two_imgs_similiarity(imgs[i], imgs[j])
    cr = get_cr(r)
    row = r.shape[0]
    ss = []
    for i in range(row):
        s = ''
        for j in range(row):
            if cr[i, j] < lam:
                cr[i, j] = 0
            else:
                cr[i, j] = 1
            s += str(cr[i, j])
        if s not in ss:
            ss.append(s)
    return r, cr, len(ss)


# 两个关系矩阵a,b的一次乘法
def get_r_times_r(a, b):
    n = a.shape[0]
    ab = np.mat(np.zeros((n, n)), 'float')
    for i in range(n):
        for j in range(n):
            mins = []
            for k in range(n):
                mins.append(min(a[i, k], b[k, j]))
            ab[i, j] = max(mins)
    return ab

# 两个关系矩阵a,b的一次合并
def get_r_merge_r(a, b):
    n = a.shape[0]
    ab = np.mat(np.zeros((n, n)), 'float')
    for i in range(n):
        for j in range(n):
            ab[i, j] = max(a[i, j], b[i, j])
    return ab


# 根据r得到闭包（平方法）
def get_cr(r):
    tr = r  # 上一次的闭包运算结果
    rr = get_r_times_r(r, r)
    trr = get_r_merge_r(r, rr)  # 当前闭包运算结果
    for i in range(2, 20):  # 乘法次数上限(理论上有限次，但为了效率考虑要控制)
        if np.all(tr == trr):
            break
        tr = trr
        rr = get_r_times_r(rr, rr)
        trr = get_r_merge_r(trr, rr)
    # print(f'已计算r的乘法{i}次')
    return trr


if __name__ == '__main__':
    rootroad = r'C:/Desktop/数模/模糊数学讲稿/数据集合并'  # 三个文件夹的照片并集
    dirs = os.listdir(rootroad)
    file = os.walk(rootroad)
    imgs = []
    for path, dir_list, file_list in file:
        for file_name in file_list:
            imgs.append(f'{rootroad}/{file_name}')
    for i in range(85, 0, -1):
        r, cr, n = get_imgs_r_cr_n(imgs, i/100)
        # print(f'当前文件夹下照片可分为{n}类, lambda={i/100}\n')
        if n == 3:
            print(f'当lambda为{i/100}时，当前文件夹下照片可分为3类')
            break
        if n == 1:
            print('分类失败，未达预期（3类）')
            break





