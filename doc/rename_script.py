import os
import sys

def rename_files_and_dirs(root_path, old_str, new_str):
    """
    递归重命名指定路径下的文件和文件夹
    
    Args:
        root_path (str): 要处理的根目录路径
        old_str (str): 需要被替换的字符串
        new_str (str): 替换后的目标字符串
    """
    # 先检查根路径是否存在
    if not os.path.exists(root_path):
        print(f"错误：路径 '{root_path}' 不存在！")
        return
    
    # 递归遍历目录，先处理子目录/文件，再处理父目录，避免路径错误
    # 先收集所有需要处理的路径，再逆序处理（保证子级先处理）
    all_items = []
    for dirpath, dirnames, filenames in os.walk(root_path, topdown=False):
        # 添加文件夹
        for dirname in dirnames:
            all_items.append((dirpath, dirname, True))  # True 标识是文件夹
        # 添加文件
        for filename in filenames:
            all_items.append((dirpath, filename, False))  # False 标识是文件
    
    # 遍历所有项进行重命名
    for dirpath, name, is_dir in all_items:
        # 检查名称是否包含需要替换的字符串
        if old_str in name:
            # 构建原路径和新路径
            old_full_path = os.path.join(dirpath, name)
            new_name = name.replace(old_str, new_str)
            new_full_path = os.path.join(dirpath, new_name)
            
            # 避免重命名后路径相同（比如替换内容不存在时）
            if old_full_path == new_full_path:
                continue
            
            try:
                # 执行重命名
                os.rename(old_full_path, new_full_path)
                item_type = "文件夹" if is_dir else "文件"
                print(f"成功重命名 {item_type}：{old_full_path} -> {new_full_path}")
            except Exception as e:
                print(f"重命名失败 {old_full_path}：{str(e)}")

def main():
    # 检查命令行参数数量
    if len(sys.argv) != 4:
        # Like: python rename_script.py 路径 Ela NX
        print("使用方法：python rename_script.py <目标路径> <需要替换的内容> <目标内容>")
        print("示例：python rename_script.py ./test_folder Ela NX")
        sys.exit(1)
    
    # 获取命令行参数
    root_path = sys.argv[1]
    old_str = sys.argv[2]
    new_str = sys.argv[3]
    
    # 执行重命名操作
    rename_files_and_dirs(root_path, old_str, new_str)
    print("\n重命名操作执行完成！")

if __name__ == "__main__":
    main()
