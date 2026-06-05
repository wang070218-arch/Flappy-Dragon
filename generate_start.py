"""
基于 FD/poster.png 生成开始页面
 - 从海报中裁剪合适的区域作为背景
 - 覆盖 "nuist Flappy Dragon" 文字
 - 输出 res/start.png（彩色+黑底）和 res/startFront.png（彩色+白底）
"""

from PIL import Image, ImageDraw, ImageFont, ImageFilter, ImageEnhance
import os

W, H = 400, 740
POSTER_PATH = r'C:\Users\1\Desktop\FD\poster.png'

def make_start_images():
    # ── 打开海报 ──
    poster = Image.open(POSTER_PATH).convert('RGBA')
    pw, ph = poster.size

    # ── 裁剪海报顶部区域（包含标题/角色） ──
    # 调整裁剪区域：从顶部开始，宽度400，高度740
    # 海报太宽，水平居中裁剪；垂直方向取顶部含标题的部分
    crop_x = (pw - W) // 2
    crop_y = 0
    cropped = poster.crop((crop_x, crop_y, crop_x + W, crop_y + H))

    # ── 稍微调亮以适应屏幕显示 ──
    enhancer = ImageEnhance.Brightness(cropped)
    cropped = enhancer.enhance(1.1)

    # ── 加载字体 ──
    font_dir = 'C:/Windows/Fonts'
    title_font = None
    sub_font = None
    hint_font = None

    for name, size in [('impact.ttf', 56), ('arialbd.ttf', 56)]:
        path = os.path.join(font_dir, name)
        if os.path.isfile(path):
            try:
                title_font = ImageFont.truetype(path, 56)
                break
            except:
                continue
    for name, size in [('arialbd.ttf', 30), ('segoeuib.ttf', 30), ('calibrib.ttf', 30)]:
        path = os.path.join(font_dir, name)
        if os.path.isfile(path):
            try:
                sub_font = ImageFont.truetype(path, 30)
                break
            except:
                continue
    for name, size in [('segoeui.ttf', 22), ('arial.ttf', 22)]:
        path = os.path.join(font_dir, name)
        if os.path.isfile(path):
            try:
                hint_font = ImageFont.truetype(path, 22)
                break
            except:
                continue

    if title_font is None:
        title_font = ImageFont.load_default()
    if sub_font is None:
        sub_font = ImageFont.load_default()
    if hint_font is None:
        hint_font = ImageFont.load_default()

    # ── 在海报上叠文字 ──
    # 用半透明黑色条增加文字可读性
    overlay = Image.new('RGBA', (W, H), (0, 0, 0, 0))
    draw = ImageDraw.Draw(overlay)

    # 顶部半透明黑条（让文字更清晰）
    draw.rectangle([0, 50, W, 220], fill=(0, 0, 0, 120))

    # Title: "Flappy Dragon"
    title = 'Flappy Dragon'
    bbox = draw.textbbox((0, 0), title, font=title_font)
    tw, th = bbox[2] - bbox[0], bbox[3] - bbox[1]
    tx, ty = (W - tw) // 2, 80

    # 文字阴影
    for ox, oy in [(2, 2), (3, 3)]:
        draw.text((tx + ox, ty + oy), title, fill=(0, 0, 0, 200), font=title_font)
    # 白色主文字
    draw.text((tx, ty), title, fill=(255, 255, 255, 255), font=title_font)

    # Subtitle: "nuist"
    sub = 'nuist'
    bbox2 = draw.textbbox((0, 0), sub, font=sub_font)
    sw = bbox2[2] - bbox2[0]
    sx, sy = (W - sw) // 2, ty + th + 12
    for ox, oy in [(1, 1), (2, 2)]:
        draw.text((sx + ox, sy + oy), sub, fill=(0, 0, 0, 200), font=sub_font)
    draw.text((sx, sy), sub, fill=(255, 215, 150, 255), font=sub_font)  # 暖金色

    # 底部半透明黑条 + "Click to Start"
    draw.rectangle([100, 600, W - 100, 660], fill=(0, 0, 0, 100))
    hint = 'Click to Start'
    bbox3 = draw.textbbox((0, 0), hint, font=hint_font)
    hw = bbox3[2] - bbox3[0]
    hx, hy = (W - hw) // 2, 618
    draw.text((hx + 1, hy + 1), hint, fill=(0, 0, 0, 200), font=hint_font)
    draw.text((hx, hy), hint, fill=(180, 220, 255, 255), font=hint_font)

    # 合成海报 + 文字层
    combined = Image.alpha_composite(cropped, overlay).convert('RGBA')

    # ── 生成 start.png（彩色+黑底） ──
    # 透明区域变黑，彩色区域保留
    color_img = Image.new('RGB', (W, H), (0, 0, 0))
    color_pixels = color_img.load()
    combined_px = combined.load()
    for y in range(H):
        for x in range(W):
            r, g, b, a = combined_px[x, y]
            if a > 128:
                color_pixels[x, y] = (r, g, b)
            else:
                color_pixels[x, y] = (0, 0, 0)
    color_img.save('res/start.png')
    print('✅ res/start.png 已生成')

    # ── 生成 startFront.png（彩色+白底） ──
    mask_img = Image.new('RGB', (W, H), (255, 255, 255))
    mask_pixels = mask_img.load()
    for y in range(H):
        for x in range(W):
            r, g, b, a = combined_px[x, y]
            if a > 128:
                mask_pixels[x, y] = (r, g, b)
            else:
                mask_pixels[x, y] = (255, 255, 255)
    mask_img.save('res/startFront.png')
    print('✅ res/startFront.png 已生成')

    print('🎉 完成！重新编译运行即可。')

if __name__ == '__main__':
    make_start_images()
