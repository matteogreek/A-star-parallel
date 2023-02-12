from PIL import Image
import glob

for filename in glob.glob('D:\\Archivio\\universita\\HighPerformanceComputing\\A-star-parallel\\input_imgs\\400x400_m.png'):
    im = Image.open(filename) # Can be many different formats.
    pix = im.load()
    print (im.size )
    print(filename)
    for x in range(im.size[1]-1):
        for y in range(im.size[0]-1):
            #print( pix[x,y]  )# Get the RGBA Value of the a pixel of an image
            if(pix[y,x][0]==0 and pix[y,x+1][0] == 0 and pix[y+1,x][0] ==0 and pix[y+1,x+1][0] ==0 ):
                    print("square detected in "+str(x)+" "+str(y))
                    #raise SystemExit
print("end")