from PIL import Image

im = Image.open('Immagine.png') # Can be many different formats.
pix = im.load()
print (im.size ) # Get the width and hight of the image for iterating over
string=""
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
    with open("13.txt","a") as f:
        f.write(string)
    string=""
print("end")
#pix[x,y] = value  # Set the RGBA Value of the image (tuple)
#im.save('alive_parrot.png')