from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import numpy as np

IS_PERSPECTIVE = True                               # 透视投影
VIEW = np.array([-20, 20, -20, 20, 10, 100])        # 视景体的left/right/bottom/top/near/far六个面
SCALE_K = np.array([1.0, 1.0, 1.0])                 # 模型缩放比例
EYE = np.array([0.0, 0.0, 40.0])                    # 眼睛的位置（默认z轴的正方向）
ORIGIN = np.array([0.0, 0.0, 0.0])                  # 原点
LOOK_AT = np.array([0.0, 0.0, 0.0])                 # 瞄准方向的参考点（默认在坐标原点）
EYE_UP = np.array([0.0, 1.0, 0.0])                  # 定义对观察者而言的上方（默认y轴的正方向）
WIN_W, WIN_H = 640, 480                             # 保存窗口宽度和高度的变量
LEFT_IS_DOWNED = False                              # 鼠标左键被按下
MOUSE_X, MOUSE_Y = 0, 0                             # 考察鼠标位移量时保存的起始位置
PEN_1 = 1
PEN_2 = 1
PEN_3 = 0
book = 1
water = 1

def getposture():

    global EYE, ORIGIN
    dist = np.sqrt(np.power((EYE-ORIGIN), 2).sum())

    if dist > 0:
        phi = np.arcsin((EYE[1]-ORIGIN[1])/dist)
        theta = np.arcsin((EYE[0]-ORIGIN[0])/(dist*np.cos(phi)))
    else:
        phi = 0.0
        theta = 0.0

    return dist, phi, theta

DIST, PHI, THETA = getposture()                     # 眼睛与观察目标之间的距离、仰角、方位角

def init():

    glClearColor(1.0, 1.0, 1.0, 1.0) # 设置画布背景色。注意：这里必须是4个参数
    glEnable(GL_DEPTH_TEST)          # 开启深度测试，实现遮挡关系
    glDepthFunc(GL_LEQUAL)           # 设置深度测试函数（GL_LEQUAL只是选项之一）

def drawbook():

    glPushMatrix()
    glTranslatef(0,0.7,3)
    glScalef(1,0.2,8)
    glutWireCube(1)   #书
    glPopMatrix()

    for i in [-1,1]:
        for j in [-2,-1,1,2]:
            glPushMatrix()
            glTranslatef(0,1.2,3+2*i+0.3*j)
            glutWireTorus(0.05,0.4,10,10)   #书圆环
            glPopMatrix()
        glPushMatrix()
        glTranslatef(4*i,0.7,3)
        glScalef(7,0.2,8)
        glutWireCube(1)   #书
        glPopMatrix()
        glPushMatrix()
        glTranslatef(-3.4*i,1,3)
        glScalef(6.5,0.4,7)
        glutWireCube(1)   #书
        glPopMatrix()

def drawbook0():

    glPushMatrix()
    glTranslatef(-3,1,3)
    glScalef(0.2,1,8)
    glutWireCube(1)   #书
    glPopMatrix()

    for i in [-1,1]:
        for j in [-2,-1,1,2]:
            glPushMatrix()
            glTranslatef(-2.6,1,3+2*i+0.3*j)
            glutWireTorus(0.05,0.4,10,10)   #书圆环
            glPopMatrix()
        glPushMatrix()
        glTranslatef(0.5,1+i*0.4,3)
        glScalef(7,0.2,8)
        glutWireCube(1)   #书
        glPopMatrix()
    
    glPushMatrix()
    glTranslatef(0.5,1,3)
    glScalef(6.5,0.6,7)
    glutWireCube(1)   #书
    glPopMatrix()

def drawdesk():

    glPushMatrix()
    glScalef(3,0.1,1.5)
    glutWireCube(10)  #桌面
    glPopMatrix()

    glPushMatrix()
    glTranslatef(0,-14,-5.5)
    glScalef(25,1,1)
    glutWireCube(1)   #栏杆
    glPopMatrix()

    for i in [-1,1]:
        for j in [-1,1]:
            glPushMatrix()
            glTranslatef(13*i,-10.5,5.5*j)
            glScalef(1.5,20,1.5)
            glutWireCube(1)   #桌腿
            glPopMatrix()
        
        glPushMatrix()
        glTranslatef(-6*i,-2.5,0)
        glScalef(11.5,4,13)
        glutWireCube(1)   #抽屉
        glPopMatrix()

        glPushMatrix()
        glTranslatef(13*i,-16,0)
        glScalef(1,1,10)
        glutWireCube(1)   #栏杆
        glPopMatrix()

def drawpen1():
    glPushMatrix()
    glTranslatef(8,0.7,0)
    glScalef(1,1,1)
    glutWireCylinder(0.25,2,100,100)   #圆柱#笔
    glPopMatrix()

    glPushMatrix()
    glTranslatef(8,0.7,2)
    glScalef(1,1,1)
    glutWireCylinder(0.18,3,100,100)   #圆柱#笔
    glPopMatrix()

def drawcup():
    glPushMatrix()
    glTranslatef(-11,1,-3)
    glScalef(1,1,1)
    glRotatef(90,1,0,0)
    glutSolidCylinder(1.3,0.5,100,100)   #圆柱
    glPopMatrix()

    glPushMatrix()
    glTranslatef(-11,4.7,-3)
    glScalef(1,1,1)
    glRotatef(90,1,0,0)
    glutWireTorus(0.1,1.5,100,100)   #圆环
    glPopMatrix()

    glPushMatrix()
    glTranslatef(-11,4.7,-3)
    glRotatef(90,1,0,0)
    cylinder = gluNewQuadric()
    gluQuadricDrawStyle(cylinder, GLU_LINE)   #圆台面
    gluCylinder(cylinder, 1.5, 1.3, 3.7, 20, 10)
    glPopMatrix()

def drawwater_full():
    
    glPushMatrix()
    glTranslatef(-11,4,-3)
    glRotatef(90,1,0,0)
    glutSolidCylinder(1.5,0.1,10,10)
    glPopMatrix()

def drawwater_poor():
    
    glPushMatrix()
    glTranslatef(-11,2,-3)
    glRotatef(90,1,0,0)
    glutSolidCylinder(1.4,0.1,10,10)
    glPopMatrix()

def drawpen2():
    glPushMatrix()
    glTranslatef(9.5,5,-4)
    glScalef(1,1,1)
    glRotatef(30,0,0,1)
    glRotatef(80,1,0,0)
    glutWireCylinder(0.25,2,100,100)   #圆柱
    glTranslatef(0,0,2)
    glScalef(1,1,1)
    glutWireCylinder(0.18,3,100,100)   #圆柱
    glPopMatrix()

def drawpen3():
    glPushMatrix()
    glTranslatef(12.2,5,-4.5)
    glScalef(1,1,1)
    glRotatef(-30,0,0,1)
    glRotatef(80,1,0,0)
    glutWireCylinder(0.25,2,100,100)   #圆柱
    glTranslatef(0,0,2)
    glScalef(1,1,1)
    glutWireCylinder(0.18,3,100,100)   #圆柱
    glPopMatrix()

def drawpencup():
    for i in [-1,1]:
        glPushMatrix()
        glTranslatef(11,2.35+i*1.65,-4)
        glScalef(1,1,1)
        glRotatef(90,1,0,0)
        glutWireTorus(0.1,1.6,100,100)   #笔筒#圆环
        glPopMatrix()

    #笔筒#面
    # ---------------------------------------------------------------
    glPushMatrix()
    glTranslatef(11,2.4,-4)
    glScalef(1,17,1)
    glRotatef(90,1,0,0)
    glutWireTorus(0.1,1.6,10,10)   #圆柱
    glPopMatrix()

def draw():

    global IS_PERSPECTIVE, VIEW
    global EYE, LOOK_AT, EYE_UP
    global SCALE_K
    global WIN_W, WIN_H
    global PEN_1,PEN_2,PEN_3
    global water

    # 清除屏幕及深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    # 设置投影（透视投影）
    glMatrixMode(GL_PROJECTION)

    glLoadIdentity()

    #调整窗口
    if WIN_W > WIN_H:
        if IS_PERSPECTIVE:
            glFrustum(VIEW[0]*WIN_W/WIN_H, VIEW[1]*WIN_W/WIN_H, VIEW[2], VIEW[3], VIEW[4], VIEW[5])
        else:
            glOrtho(VIEW[0]*WIN_W/WIN_H, VIEW[1]*WIN_W/WIN_H, VIEW[2], VIEW[3], VIEW[4], VIEW[5])
    else:
        if IS_PERSPECTIVE:
            glFrustum(VIEW[0], VIEW[1], VIEW[2]*WIN_H/WIN_W, VIEW[3]*WIN_H/WIN_W, VIEW[4], VIEW[5])
        else:
            glOrtho(VIEW[0], VIEW[1], VIEW[2]*WIN_H/WIN_W, VIEW[3]*WIN_H/WIN_W, VIEW[4], VIEW[5])


    # 设置模型视图
    glMatrixMode(GL_MODELVIEW)

    glLoadIdentity()

    # 几何变换
    glScale(SCALE_K[0], SCALE_K[1], SCALE_K[2])

    # 设置视点
    gluLookAt(
        EYE[0], EYE[1], EYE[2], 
        LOOK_AT[0], LOOK_AT[1], LOOK_AT[2],
        EYE_UP[0], EYE_UP[1], EYE_UP[2]
    )

    # 设置视口
    glViewport(0, 0, WIN_W, WIN_H)

    # ---------------------------------------------------------------
    #绘制
    glColor4f(0.5, 0.5, 0.5, 0)

    drawdesk()
    drawcup()
    drawpencup()

    if water == 1:
        drawwater_full()
    if water == 0:
        drawwater_poor()

    if PEN_1 == 1:
        drawpen1()
    if PEN_2 == 1:
        drawpen2()
    if PEN_3 == 1:
        drawpen3()

    if book == 1:
        drawbook()
    if book == 0:
        drawbook0()


    glutSwapBuffers()                    # 切换缓冲区，以显示绘制内容

def reshape(width, height):

    global WIN_W, WIN_H
    WIN_W, WIN_H = width, height
    glutPostRedisplay()

def mouseclick(button, state, x, y):

    global SCALE_K
    global LEFT_IS_DOWNED
    global MOUSE_X, MOUSE_Y

    MOUSE_X, MOUSE_Y = x, y

    if button == GLUT_LEFT_BUTTON:
        LEFT_IS_DOWNED = state==GLUT_DOWN
    elif button == 3:
        SCALE_K *= 1.05
        glutPostRedisplay()
    elif button == 4:
        SCALE_K *= 0.95
        glutPostRedisplay()

def mousemotion(x, y):

    global LEFT_IS_DOWNED
    global EYE, EYE_UP
    global MOUSE_X, MOUSE_Y
    global DIST, PHI, THETA
    global DIST_1, PHI_1, THETA_1
    global WIN_W, WIN_H

    if LEFT_IS_DOWNED:

        dx = MOUSE_X - x
        dy = y - MOUSE_Y
        MOUSE_X, MOUSE_Y = x, y
        PHI += 2*np.pi*dy/WIN_H
        PHI %= 2*np.pi
        THETA += 2*np.pi*dx/WIN_W
        THETA %= 2*np.pi
        r = DIST*np.cos(PHI)

        EYE[1] = DIST*np.sin(PHI)
        EYE[0] = r*np.sin(THETA)
        EYE[2] = r*np.cos(THETA)

        if 0.5*np.pi < PHI < 1.5*np.pi:
            EYE_UP[1] = -1.0
        else:
            EYE_UP[1] = 1.0

        glutPostRedisplay()

def keydown(key, x, y):

    global DIST, PHI, THETA
    global EYE, LOOK_AT, EYE_UP
    global IS_PERSPECTIVE, VIEW
    global PEN_1,PEN_2,PEN_3

    if key in [b'x', b'X', b'y', b'Y', b'z', b'Z']:
        if key == b'x': # 瞄准参考点 x 减小
            LOOK_AT[0] -= 0.5
        elif key == b'X': # 瞄准参考 x 增大
            LOOK_AT[0] += 0.5
        elif key == b'y': # 瞄准参考点 y 减小
            LOOK_AT[1] -= 0.5
        elif key == b'Y': # 瞄准参考点 y 增大
            LOOK_AT[1] += 0.5
        elif key == b'z': # 瞄准参考点 z 减小
            LOOK_AT[2] -= 0.5
        elif key == b'Z': # 瞄准参考点 z 增大
            LOOK_AT[2] += 0.5

        DIST, PHI, THETA = getposture()
        glutPostRedisplay()
    
    elif key == b'+':
        if PEN_1 == 0:
            PEN_1 = 1
        elif PEN_2 == 0:
            PEN_2 = 1
        elif PEN_3 == 0:
            PEN_3 = 1
        glutPostRedisplay()
    elif key == b'1':
        if PEN_1 == 1:
            PEN_1 = 0
        else:
            PEN_1 = 1       
        glutPostRedisplay()
    
    elif key == b'2':
        if PEN_2 == 1:
            PEN_2 = 0
        else:
            PEN_2 = 1      
        glutPostRedisplay()
    
    elif key == b'3':
        if PEN_3 == 1:
            PEN_3 = 0
        else:
            PEN_3 = 1
        glutPostRedisplay()
    

    elif key == b'\r': # 回车键，视点前进
        EYE = LOOK_AT + (EYE - LOOK_AT) * 0.9
        DIST, PHI, THETA = getposture()
        glutPostRedisplay()

    elif key == b'\x08': # 退格键，视点后退
        EYE = LOOK_AT + (EYE - LOOK_AT) * 1.1
        DIST, PHI, THETA = getposture()
        glutPostRedisplay()

    elif key == b' ': # 空格键，切换投影模式
        IS_PERSPECTIVE = not IS_PERSPECTIVE 
        glutPostRedisplay()

    elif key == b'w' or key == b'W': # w键，视点向上
        EYE = EYE - EYE_UP
        LOOK_AT = LOOK_AT - EYE_UP
        DIST, PHI, THETA = getposture()
        glutPostRedisplay()

    elif key == b'a' or key == b'A': # a键，视点向左
        Dt = np.cross(EYE_UP,EYE-LOOK_AT)
        Dx = Dt/np.linalg.norm(Dt)
        EYE = EYE + Dx
        LOOK_AT = LOOK_AT + Dx
        DIST, PHI, THETA = getposture()
        glutPostRedisplay()

    elif key == b's' or key == b'S': # s键，视点向下
        EYE = EYE + EYE_UP
        LOOK_AT = LOOK_AT + EYE_UP
        DIST, PHI, THETA = getposture()
        glutPostRedisplay()

    elif key == b'd' or key == b'D': # d键，视点向右
        Dt = np.cross(EYE_UP,EYE-LOOK_AT)
        Dx = Dt/np.linalg.norm(Dt)
        EYE = EYE - Dx
        LOOK_AT = LOOK_AT - Dx
        DIST, PHI, THETA = getposture()
        glutPostRedisplay()

def menufunc(data):
    global book
    global water

    if data == 1:
        book = 1
    elif data == 0:
        book = 0
    elif data == 3:
        water = 1
    elif data == 2:
        water = 0
    glutPostRedisplay()

if __name__ == "__main__":

    glutInit()
    displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH
    glutInitDisplayMode(displayMode)
    glutInitWindowSize(WIN_W, WIN_H)
    glutInitWindowPosition(300, 200)
    glutCreateWindow('desk of cx')

    init()                              # 初始化画布
    glutDisplayFunc(draw)               # 注册回调函数draw()
    glutReshapeFunc(reshape)            # 注册响应窗口改变的函数reshape()
    glutMouseFunc(mouseclick)           # 注册响应鼠标点击的函数mouseclick()
    glutMotionFunc(mousemotion)         # 注册响应鼠标拖拽的函数mousemotion()
    glutKeyboardFunc(keydown)           # 注册键盘输入的函数keydown()

    menu = glutCreateMenu(menufunc) # 注册菜单函数menufunc
    glutAddMenuEntry('read book', 1)
    glutAddMenuEntry('close book', 0)
    glutAddMenuEntry('drink water', 2)
    glutAddMenuEntry('add water', 3)
    glutAttachMenu(GLUT_RIGHT_BUTTON)

    glutMainLoop()                      # 进入glut主循环    