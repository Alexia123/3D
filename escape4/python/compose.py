from PIL import Image
back = Image.open('image.png', 'r')
fore = Image.open('espion.png', 'r')
back.paste(fore, (0,0), fore)
back.save('out.png')
