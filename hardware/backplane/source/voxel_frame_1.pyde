add_library('svg')

def setup():
    beginRecord(SVG, "filename.svg")    
    

    
    size(800, 1000)
    background(255)  
    fill(0,0)
    
    s=60 # size in pixels of a side of one square

    physical = 31.5 #mm of a side of one square
    dpi = s/25.4*physical # 1 dot per inch == 1/25.4 dots per mm
    print("scale with dpi",dpi)    
    
    b=10 # border thickness between 
    ss=30 # size of small rectangles
    nx=6
    ny=8
    translate(s, s*4)
    
    push()
    for y in range(0,ny):
        push()
        for x in range(0,nx):
            fill(0,0)
            rect(0,0,s,s)
            translate(s+b,0)
            translate(0,-ss)
        pop()
        if y==2 or y==4:
            translate(-s-b,ss)
        translate(0,s+b) #next row
        translate(ss,0)
    pop()
    # draw line around
    fill(0,0)
    rect(-b-b,-s-s-s,(s+b)*(nx+1)+b*3,(s+b)*(ny+4))
    # translate(-b*2,-b*2)
    # for x in range(0,nx):   
    #     line(0,0,s+b,0)
    #     line(s+b,0,s+b,-ss)
    #     translate(s+b,-ss)
    
    endRecord()
    
def polygon(x, y, radius, npoints):
    angle = TWO_PI / npoints
    with beginClosedShape():
        a = 0
        while a < TWO_PI:
            sx = x + cos(a) * radius
            sy = y + sin(a) * radius
            vertex(sx, sy)
            a += angle
