import tkinter as tk
from tkinter import filedialog, ttk, messagebox
from PIL import Image, ImageTk, ImageEnhance
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import cv2
import matplotlib

# 设置matplotlib后端为TkAgg：将 Matplotlib 绘制的图形嵌入到 Tkinter 的 GUI 窗口中，实现动态交互式显示
plt.switch_backend('TkAgg')

# 支持中文显示
matplotlib.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'SimSun', 'Arial Unicode MS']

class ImageProcessor:
    def __init__(self, root):
        self.root = root
        self.root.title("图像处理工具")
        self.root.geometry("1100x900")
        self.root.configure(bg="#f6fcff")
        
        # 设置样式
        self.style = ttk.Style()
        self.style.configure("TButton", font=("微软雅黑", 11))
        self.style.configure("TLabel", font=("微软雅黑", 11))
        self.style.configure("TRadiobutton", font=("微软雅黑", 11))
        self.style.configure("TCheckbutton", font=("微软雅黑", 11))
        self.style.configure("TFrame", background="#f6fcff")
        
        # 初始化变量
        self.original_image = None # 原始图像
        self.processed_image = None # 处理后图像
        self.original_path = None # 原始图像路径
        self.histogram_figure = None # 直方图
        self.histogram_canvas = None
        
        # 处理选项变量
        self.histogram_enabled = tk.BooleanVar(value=False) # 直方图均衡化
        self.brightness_var = tk.DoubleVar(value=1.0) # 亮度
        self.contrast_var = tk.DoubleVar(value=1.0) # 对比度
        self.red_var = tk.DoubleVar(value=1.0) # RGB颜色调整
        self.green_var = tk.DoubleVar(value=1.0)
        self.blue_var = tk.DoubleVar(value=1.0)
        
        # 创建主框架
        self.main_frame = ttk.Frame(self.root, padding="10")
        self.main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 创建按钮、图像显示、处理区和直方图显示
        self.create_buttons()
        self.create_image_display()
        self.create_processing_and_histogram()
        
    def create_buttons(self):
        button_frame = ttk.Frame(self.main_frame)
        button_frame.grid(row=0, column=0, columnspan=2, padx=5, pady=5, sticky=(tk.W, tk.E))
        
        # 加载/保存/重置按钮
        self.load_btn = ttk.Button(button_frame, text="加载图像", command=self.load_image)
        self.load_btn.grid(row=0, column=0, padx=5, pady=5)
        self.save_btn = ttk.Button(button_frame, text="保存图像", command=self.save_image)
        self.save_btn.grid(row=0, column=1, padx=5, pady=5)
        self.reset_btn = ttk.Button(button_frame, text="重置图像", command=self.reset_all)
        self.reset_btn.grid(row=0, column=2, padx=5, pady=5)
        
    def create_image_display(self):
        image_frame = ttk.Frame(self.main_frame)
        image_frame.grid(row=1, column=0, columnspan=2, padx=5, pady=5, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 原始图像显示
        self.original_frame = ttk.LabelFrame(image_frame, text="原始图像", padding="5")
        self.original_frame.grid(row=0, column=0, padx=5, pady=5, sticky=(tk.W, tk.E, tk.N, tk.S))
        self.original_label = ttk.Label(self.original_frame)
        self.original_label.grid(row=0, column=0)
        
        # 处理后图像显示
        self.processed_frame = ttk.LabelFrame(image_frame, text="处理后图像", padding="5")
        self.processed_frame.grid(row=0, column=1, padx=5, pady=5, sticky=(tk.W, tk.E, tk.N, tk.S))
        self.processed_label = ttk.Label(self.processed_frame)
        self.processed_label.grid(row=0, column=0)
        
    def create_processing_and_histogram(self):
        processing_histogram_frame = ttk.Frame(self.main_frame)
        processing_histogram_frame.grid(row=2, column=0, columnspan=2, padx=5, pady=5, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 创建处理选项/直方图
        self.create_processing_options(processing_histogram_frame)
        self.create_histogram_display(processing_histogram_frame)
        
    def create_processing_options(self, parent_frame):
        options_frame = ttk.LabelFrame(parent_frame, text="处理选项", padding="5")
        options_frame.grid(row=0, column=0, padx=5, pady=5, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 直方图均衡化选项
        histogram_frame = ttk.Frame(options_frame)
        histogram_frame.grid(row=0, column=0, padx=5, pady=5, sticky=(tk.W, tk.E))
        self.histogram=ttk.Checkbutton(histogram_frame, text="启用直方图均衡化", variable=self.histogram_enabled, command=self.apply_processing)
        self.histogram.grid(row=0, column=0, padx=5, sticky=tk.W)
        
        # 亮度/对比度滑块
        brightness_frame = ttk.LabelFrame(options_frame, text="亮度/对比度调整", padding="5")
        brightness_frame.grid(row=1, column=0, padx=5, pady=5, sticky=(tk.W, tk.E))
        
        ttk.Label(brightness_frame, text="亮度:").grid(row=0, column=0, padx=5, sticky=tk.W)
        self.brightness_scale = ttk.Scale(brightness_frame, from_=0.0, to=2.0, variable=self.brightness_var, orient=tk.HORIZONTAL, command=self.apply_processing)
        self.brightness_scale.grid(row=0, column=1, padx=5, sticky=(tk.W, tk.E))
        
        ttk.Label(brightness_frame, text="对比度:").grid(row=1, column=0, padx=5, sticky=tk.W)
        self.contrast_scale = ttk.Scale(brightness_frame, from_=0.0, to=2.0, variable=self.contrast_var, orient=tk.HORIZONTAL, command=self.apply_processing)
        self.contrast_scale.grid(row=1, column=1, padx=5, sticky=(tk.W, tk.E))
        
        # 亮度/对比度重置按钮
        ttk.Button(brightness_frame, text="重置亮度/对比度", command=self.reset_brightness_contrast).grid(row=2, column=0, columnspan=2, padx=5, pady=5)
        
        # 颜色调整滑块
        color_frame = ttk.LabelFrame(options_frame, text="颜色调整", padding="5")
        color_frame.grid(row=2, column=0, padx=5, pady=5, sticky=(tk.W, tk.E))
        
        ttk.Label(color_frame, text="红色:").grid(row=0, column=0, padx=5, sticky=tk.W)
        self.red_scale = ttk.Scale(color_frame, from_=0.0, to=2.0, variable=self.red_var, orient=tk.HORIZONTAL, command=self.apply_processing)
        self.red_scale.grid(row=0, column=1, padx=5, sticky=(tk.W, tk.E))
        
        ttk.Label(color_frame, text="绿色:").grid(row=1, column=0, padx=5, sticky=tk.W)
        self.green_scale = ttk.Scale(color_frame, from_=0.0, to=2.0, variable=self.green_var, orient=tk.HORIZONTAL, command=self.apply_processing)
        self.green_scale.grid(row=1, column=1, padx=5, sticky=(tk.W, tk.E))
        
        ttk.Label(color_frame, text="蓝色:").grid(row=2, column=0, padx=5, sticky=tk.W)
        self.blue_scale = ttk.Scale(color_frame, from_=0.0, to=2.0, variable=self.blue_var, orient=tk.HORIZONTAL, command=self.apply_processing)
        self.blue_scale.grid(row=2, column=1, padx=5, sticky=(tk.W, tk.E))
        
        # 颜色重置按钮
        ttk.Button(color_frame, text="重置颜色", command=self.reset_color).grid(row=3, column=0, columnspan=2, padx=5, pady=5)
        
    def create_histogram_display(self, parent_frame):
        histogram_frame = ttk.LabelFrame(parent_frame, text="直方图", padding="5")
        histogram_frame.grid(row=0, column=1, padx=5, pady=5, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 创建matplotlib图形
        self.histogram_figure = plt.Figure(figsize=(8, 4), dpi=100)
        self.histogram_canvas = FigureCanvasTkAgg(self.histogram_figure, master=histogram_frame)
        self.histogram_canvas.get_tk_widget().grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
    def reset_all(self):
        # 重置所有处理选项
        self.histogram_enabled.set(False)
        self.reset_brightness_contrast()
        self.reset_color()
        
        # 重置图像
        if self.original_image is not None:
            self.processed_image = self.original_image.copy()
            self.display_image(self.processed_image, self.processed_label)
            self.update_histogram()
            
    def reset_brightness_contrast(self):
        # 重置亮度和对比度
        self.brightness_var.set(1.0)
        self.contrast_var.set(1.0)
        
        if self.original_image is not None:
            self.apply_processing()
            
    def reset_color(self):
        # 重置颜色
        self.red_var.set(1.0)
        self.green_var.set(1.0)
        self.blue_var.set(1.0)
        
        if self.original_image is not None:
            self.apply_processing()
            
    def load_image(self):
        file_path = filedialog.askopenfilename(filetypes=[
            ("图像文件", "*.jpg *.png *.bmp")
        ])
        if file_path:
            self.original_path = file_path
            self.original_image = Image.open(file_path)
            self.processed_image = self.original_image.copy()
            self.display_image(self.original_image, self.original_label)
            self.display_image(self.processed_image, self.processed_label)
            self.update_histogram()
            
    def save_image(self):
        if self.processed_image is None:
            messagebox.showinfo("提示", "[Warning] 没有可保存的图像")
            return
            
        file_path = filedialog.asksaveasfilename(defaultextension=".png", filetypes=[
            ("PNG图像", "*.png"),
            ("JPEG图像", "*.jpg"),
            ("BMP图像", "*.bmp")
        ])
        if file_path:
            self.processed_image.save(file_path)
            messagebox.showinfo("成功", "[success] 图像已保存到 " + file_path)
            
    def apply_processing(self, *args):
        if self.original_image is None:
            return
            
        # 从原始图像开始处理
        self.processed_image = self.original_image.copy()
        if self.histogram_enabled.get():
            self.apply_histogram_equalization()
        self.apply_brightness_contrast()
        self.apply_color_adjustment()
        self.display_image(self.processed_image, self.processed_label)
        self.update_histogram()
        
    def apply_histogram_equalization(self):
        # 转换为OpenCV格式
        img_cv = np.array(self.processed_image)
        
        # 分离通道
        if len(img_cv.shape) == 3:
            # 彩色图像
            img_yuv = cv2.cvtColor(img_cv, cv2.COLOR_RGB2YUV)
            img_yuv[:,:,0] = cv2.equalizeHist(img_yuv[:,:,0])
            img_cv = cv2.cvtColor(img_yuv, cv2.COLOR_YUV2RGB)
        else:
            # 灰度图像
            img_cv = cv2.equalizeHist(img_cv)
            
        # 转回PIL格式
        self.processed_image = Image.fromarray(img_cv)
        
    def apply_brightness_contrast(self):
        # 亮度调整
        brightness_factor = self.brightness_var.get()
        enhancer = ImageEnhance.Brightness(self.processed_image)
        temp_image = enhancer.enhance(brightness_factor)
        
        # 对比度调整
        contrast_factor = self.contrast_var.get()
        enhancer = ImageEnhance.Contrast(temp_image) # 在调整亮度基础上进行调整
        self.processed_image = enhancer.enhance(contrast_factor)
        
    def apply_color_adjustment(self):
        # 颜色调整
        red_factor = self.red_var.get()
        green_factor = self.green_var.get()
        blue_factor = self.blue_var.get()
        
        # 分离通道
        r, g, b = self.processed_image.split()
        
        # 调整各通道
        r = r.point(lambda i: i * red_factor)
        g = g.point(lambda i: i * green_factor)
        b = b.point(lambda i: i * blue_factor)
        
        # 合并通道
        self.processed_image = Image.merge('RGB', (r, g, b))
        
    def update_histogram(self):
        if self.processed_image is None or self.histogram_figure is None:
            return
            
        # 清除之前的直方图
        self.histogram_figure.clear()
        
        # 转换为numpy数组
        img_array = np.array(self.processed_image)
        
        # 创建子图
        ax = self.histogram_figure.add_subplot(111)
        
        # 绘制直方图
        if len(img_array.shape) == 3:
            # 彩色图像，形状为 (height, width, 3)
            color = ('r', 'g', 'b')
            for i, col in enumerate(color):
                hist = cv2.calcHist([img_array], [i], None, [256], [0, 256]) # 计算像素块数量
                ax.plot(hist, color=col)
        else:
            # 灰度图像，形状为 (height, width)
            ax.hist(img_array.ravel(), 256, [0, 256])
            
        # 设置标题和标签
        ax.set_xlabel('像素值')
        ax.set_ylabel('频率')
        
        # 设置图形布局
        self.histogram_figure.tight_layout()
        
        # 更新画布
        self.histogram_canvas.draw()
        
    def display_image(self, image, label):
        # 限制图像高度
        window_height = self.root.winfo_height()
        max_height = window_height // 3
        
        # 计算图像显示大小，保持宽高比
        display_size = (400, min(400, max_height))
        display_image = image.copy()
        display_image.thumbnail(display_size, Image.Resampling.LANCZOS)
        photo = ImageTk.PhotoImage(display_image)
        label.configure(image=photo)
        label.image = photo

if __name__ == "__main__":
    root = tk.Tk()
    app = ImageProcessor(root)
    root.mainloop() 