from PIL import Image
import glob

for filename in glob.glob('D:\\Archivio\\universita\\HighPerformanceComputing\\A-star-parallel\\input_imgs\\*.png'):
    string=""
    im = Image.open(filename) # Can be many different formats.
    pix = im.load()
    print (im.size ) # Get the width and hight of the image for iterating over
    f=open(filename+".txt","w")
    for x in range(im.size[1]):
        for y in range(im.size[0]):
            #print( pix[x,y]  )# Get the RGBA Value of the a pixel of an image
            if(pix[y,x][0]==255):
                string+="1,"
            else:
                string+="0,"
        s=list(string)
        s[len(string)-1]="\n"
        string="".join(s)
        
    f.write("dim=(1000,1000)\nsrc=(0,0)\ndst=(0,0)\n")
    f.write(string)
    print(filename+".txt created")
print("end")
#pix[x,y] = value  # Set the RGBA Value of the image (tuple)
#im.save('alive_parrot.png')