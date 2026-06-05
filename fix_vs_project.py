"""将 IntroVideo.cpp/.h 加入 Flappy Dragon VS 项目"""
import re, os, shutil

base = r'C:\Users\1\Desktop\Flappy Dragon\Flappy Dragon'
proj_file = os.path.join(base, 'Flappy Dragon.vcxproj')

with open(proj_file, 'r', encoding='utf-8') as f:
    content = f.read()

# 1. 添加 IntroVideo.cpp 到编译列表（在 main.cpp 后面）
old_cpp = r'..\..\Flappy-Bird-EasyX-main\GameFrame\main.cpp</ClCompile>'
new_cpp = old_cpp + '\n    <ClCompile Include="..\\..\\Flappy-Bird-EasyX-main\\GameFrame\\IntroVideo.cpp" />'

if 'IntroVideo.cpp' not in content:
    content = content.replace(old_cpp, new_cpp)
    print('✅ IntroVideo.cpp 已加入项目')
else:
    print('⏭️ IntroVideo.cpp 已在项目中')

# 2. 添加 IntroVideo.h 到头文件列表（在 frame.h 后面）
old_h = r'..\..\Flappy-Bird-EasyX-main\GameFrame\frame.h</ClInclude>'
new_h = old_h + '\n    <ClInclude Include="..\\..\\Flappy-Bird-EasyX-main\\GameFrame\\IntroVideo.h" />'

if 'IntroVideo.h' not in content:
    content = content.replace(old_h, new_h)
    print('✅ IntroVideo.h 已加入项目')
else:
    print('⏭️ IntroVideo.h 已在项目中')

with open(proj_file, 'w', encoding='utf-8') as f:
    f.write(content)

# 3. 复制 intro.mp4 到 Flappy Dragon 项目目录
src_mp4 = r'C:\Users\1\Desktop\Flappy-Bird-EasyX-main\intro.mp4'
dst_mp4 = os.path.join(base, 'intro.mp4')
if not os.path.exists(dst_mp4):
    shutil.copy(src_mp4, dst_mp4)
    print('✅ intro.mp4 已复制到 Flappy Dragon 项目')
else:
    print('⏭️ intro.mp4 已存在')

print('\n🎉 全部完成！现在在 VS 里直接重新编译即可。')
