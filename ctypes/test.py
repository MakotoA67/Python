# _*_ coding: utf-8 -*-
# test.py : C言語で記述したライブラリ関数を呼び出すPythonサンプルプログラム
# 
import ctypes
from PIL import Image

lib = ctypes.cdll.LoadLibrary("./libcolorbar.so")

lib.get_color_bar_image.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)]
lib.get_color_bar_image.restype = ctypes.c_void_p

def main():

    refHeight = ctypes.c_int32()
    refWidth = ctypes.c_int32()
    img_pointer = lib.get_color_bar_image(refHeight, refWidth)

    if img_pointer is not None:
        print('Cからちゃんとポインタが戻ってきました')
        width = refWidth.value
        height = refHeight.value

        # ポインタからデータをバイト配列に変換
        buffer_size = width * height

        # ctypes.string_atを使用してポインタからデータを取得
        red_data = ctypes.string_at(img_pointer, buffer_size)
        green_data = ctypes.string_at(img_pointer + buffer_size, buffer_size)
        blue_data = ctypes.string_at(img_pointer + 2 * buffer_size, buffer_size)

        # R, G, B のデータを結合して画像にする
        img = Image.merge("RGB", (
            Image.frombytes('L', (width, height), red_data),
            Image.frombytes('L', (width, height), green_data),
            Image.frombytes('L', (width, height), blue_data)
        ))

        #img.save("output.jpg")
        #print(f'JPEGファイルで出力したよ(画像サイズ : {width} x {height})')
        #img.save("output.tiff")
        #print(f'TIFFファイルで出力したよ(画像サイズ : {width} x {height})')

        # 画像ファイルとして保存
        format = input("出力したいフォーマットの拡張子を入力して下さい: ")
        img.save(f"output.{format}")
        print(f'{format}ファイルで出力しました(画像サイズ : {width} x {height})')
        
if __name__ == "__main__":
    main()
