"""
Flappy Bird 吉祥物替换脚本
用法：把吉祥物 PNG 图片放到本脚本同级目录，然后运行：
    python generate_mascot.py

脚本会自动：
1. 读取吉祥物图片（默认 mascot.png，可用 --input 指定）
2. 缩放/居中裁剪到 56×48
3. 生成 8 帧轻微旋转的动画
4. 为每帧生成彩色图 + 遮罩图
5. 保存到 res/ 目录，覆盖原来的小鸟图片
"""

import sys
import os
from PIL import Image

# ─── 配置 ───────────────────────────────────────────
WIDTH = 56
HEIGHT = 48
OUTPUT_DIR = os.path.join(os.path.dirname(__file__), "res")
# 每帧旋转角度（让吉祥物有轻微扇动效果）
ROTATION_ANGLES = [-6, -4, -2, 0, 2, 4, 6, 8]


def make_frames(src_path: str):
    """读取源图，返回 8 帧 PIL Image（RGBA 模式，已缩放到 56×48）"""
    img = Image.open(src_path).convert("RGBA")

    # 等比例缩放，居中裁剪到 56×48
    img_ratio = img.width / img.height
    target_ratio = WIDTH / HEIGHT

    if img_ratio > target_ratio:
        # 原图更宽：按高度缩放，水平裁剪
        new_h = HEIGHT
        new_w = int(HEIGHT * img_ratio)
    else:
        # 原图更高：按宽度缩放，垂直裁剪
        new_w = WIDTH
        new_h = int(WIDTH / img_ratio)

    img = img.resize((new_w, new_h), Image.LANCZOS)

    # 居中裁剪
    left = (new_w - WIDTH) // 2
    top = (new_h - HEIGHT) // 2
    img = img.crop((left, top, left + WIDTH, top + HEIGHT))

    # 如果原图没有透明通道，把白色背景视为透明
    if img.mode == "RGBA" and img.getextrema()[-1] == (255, 255, 255, 255):
        # 没有透明度，尝试把纯白变透明
        datas = img.getdata()
        new_data = []
        for item in datas:
            if item[:3] == (255, 255, 255):
                new_data.append((255, 255, 255, 0))
            else:
                new_data.append(item)
        img.putdata(new_data)

    # 生成 8 帧（轻微旋转）
    frames = []
    for angle in ROTATION_ANGLES:
        rot = img.rotate(angle, expand=False, fillcolor=(0, 0, 0, 0))
        frames.append(rot)

    return frames


def save_frame_pair(frame: Image.Image, idx: int):
    """
    保存一帧的两张图：
      {idx}.png        → 彩色吉祥物 + 黑色背景（用于 SRCPAINT）
      {idx}Front.png   → 黑色吉祥物 + 白色背景（用于 SRCAND）
    """
    # 确保 frame 是 RGBA
    frame = frame.convert("RGBA")
    pixels = frame.load()

    # ── 彩色图：吉祥物保留颜色，透明区域变纯黑 ──
    color_img = Image.new("RGB", (WIDTH, HEIGHT), (0, 0, 0))
    color_pixels = color_img.load()
    for y in range(HEIGHT):
        for x in range(WIDTH):
            r, g, b, a = pixels[x, y]
            if a > 128:  # 不透明 → 保留颜色
                color_pixels[x, y] = (r, g, b)
            else:  # 透明 → 黑色（不绘制）
                color_pixels[x, y] = (0, 0, 0)

    # ── 遮罩图：彩色小龙 + 白底 ──
    mask_img = Image.new("RGB", (WIDTH, HEIGHT), (255, 255, 255))
    mask_pixels = mask_img.load()
    for y in range(HEIGHT):
        for x in range(WIDTH):
            r, g, b, a = pixels[x, y]
            if a > 128:  # 不透明 → 保留颜色
                mask_pixels[x, y] = (r, g, b)
            else:  # 透明 → 白色背景
                mask_pixels[x, y] = (255, 255, 255)

    # 保存
    color_path = os.path.join(OUTPUT_DIR, f"{idx}.png")
    mask_path = os.path.join(OUTPUT_DIR, f"{idx}Front.png")

    color_img.save(color_path, "PNG")
    mask_img.save(mask_path, "PNG")

    return color_path, mask_path


def main():
    # 解析参数
    src = "mascot.png"
    if len(sys.argv) > 1:
        src = sys.argv[1]

    if not os.path.isfile(src):
        print(f"❌ 找不到图片：{src}")
        print(f"用法：python generate_mascot.py [吉祥物图片路径]")
        print(f"默认读取当前目录下的 mascot.png")
        sys.exit(1)

    if not os.path.isdir(OUTPUT_DIR):
        print(f"❌ 找不到输出目录：{OUTPUT_DIR}")
        print("请确保脚本放在 Flappy-Bird-EasyX-main 目录下运行")
        sys.exit(1)

    print(f"📖 读取源图：{src}")
    frames = make_frames(src)
    print(f"✅ 生成 {len(frames)} 帧动画")

    for i, frame in enumerate(frames):
        c, m = save_frame_pair(frame, i)
        print(f"   [{i}] → {os.path.basename(c)} / {os.path.basename(m)}")

    print("\n🎉 全部完成！重新编译运行游戏即可看到吉祥物。")


if __name__ == "__main__":
    main()
