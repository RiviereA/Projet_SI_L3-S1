#include <cstdio>
#include <cstdlib>
/****************************************************************/
/* works on mac using freeglut and xquartz                      */
/* you can install freeglut using homebrew                      */
/* xquartz can be downloaded at http://xquartz.macosforge.org   */
/****************************************************************/
#include <GL/freeglut.h>
#include <jpeglib.h>
#include <jerror.h>
#include <math.h>

#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif


char presse;
int anglex,angley,x,y,xold,yold;

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void touche(int touche,int x,int y);
void souris(int boutton, int etat,int x,int y);
void sourismouv(int x,int y);
unsigned char *loadJpegImage(char *fichier, int limg, int himg);
void calculTab();
void rotation();
void rotationPioche();
void Marche();
void DAB();

void idle();
double X=0;
double XPioche=0;

double rapidite=0.5;
bool PiocheB=true;
bool PiocheM=false;

bool MarcheB=true;
bool MarcheM=false;

bool DABB=true;
bool DABM=false;

int camera=20;

double JTPDroite,TPDroite;
double JTPGauche,TPGauche;
double JointureY,JointureX;
double BrasDroitY,BrasDroitZ;
double AvantBrasDroitY,AvantBrasDroitZ;
double BrasGaucheY,BrasGaucheZ;
double AvantBrasGaucheY,AvantBrasGaucheZ;
double TeteX,TeteY,TeteZ;
double By,Bz,Rx,Ry;

int main(int argc,char **argv)
{  /* Chargement de la texture */

  /* Creation de la fenetre OpenGL */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800,800);
  glutCreateWindow("Texture JPEG");

  /* Initialisation de l'etat d'OpenGL */
  glClearColor(0.5,0.5,0.5,1);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);

  /* Mise en place de la projection perspective */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,1,1.0,5.0);
  glMatrixMode(GL_MODELVIEW);

  /* Parametrage du placage de textures */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);



  /* Mise en place des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutSpecialFunc(touche);
  glutMouseFunc(souris);
  glutMotionFunc(sourismouv);
  glutIdleFunc(rotation);

  /* Entr?e dans la boucle principale glut */
  glutMainLoop();
  return 0;
}


void drawFloor(int f)
    {
    glEnable(GL_TEXTURE_2D);

    unsigned char* image = loadJpegImage("./sol.jpg",1024,1024);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE,image);
        glColor3f(1,1,1);
        glBegin(GL_POLYGON);
        glTexCoord2f(1,1);       glVertex3f(-f+1,0,-f+1);
        glTexCoord2f(0,1);       glVertex3f(-f+1,0,f+1);
        glTexCoord2f(0,0);       glVertex3f(f+1,0,f+1);
        glTexCoord2f(1,0);       glVertex3f(f+1,0,-f+1);
        glEnd();
    glDisable(GL_TEXTURE_2D);
    }
void rotation(){
    X=X+2;
    if(X==360) X=0;

    rotationPioche();

    Marche();

    DAB();

    glutPostRedisplay();
}


void rotationPioche(){
    if(PiocheM||XPioche!=0){
    if(PiocheB) XPioche=XPioche+1*rapidite;
    else XPioche=XPioche-1*rapidite;
    if (XPioche>30) PiocheB = false;
    if (XPioche<-70) PiocheB= true;

    glutPostRedisplay();
    }
}

void Marche(){
    if(MarcheM||JTPGauche!=0){

    if(MarcheB)
    {
      JTPDroite=JTPDroite-1*rapidite;
      JTPGauche=JTPGauche+1*rapidite;
      TPGauche=TPGauche+0.5*rapidite;
      TPDroite=TPDroite-0.5*rapidite;
    }
    else
    {
      JTPDroite=JTPDroite+1*rapidite;
      JTPGauche=JTPGauche-1*rapidite;
      TPGauche=TPGauche-0.5*rapidite;
      TPDroite=TPDroite+0.5*rapidite;
    }
    if (JTPGauche>80) MarcheB = false;
    if (JTPGauche<-80) MarcheB= true;

    glutPostRedisplay();
    }
}
void DAB(){
    if(DABM||AvantBrasGaucheY!=0){

    if(DABB)
    {
          BrasDroitZ=BrasDroitZ-0.5*rapidite;//-30
          BrasDroitY=BrasDroitY+0.15*rapidite;//10
          BrasGaucheY=BrasGaucheY+1*rapidite;//80
          BrasGaucheZ=BrasGaucheZ-0.3*rapidite;//-20
          AvantBrasGaucheY=AvantBrasGaucheY+1.5*rapidite;//120
          AvantBrasGaucheZ=AvantBrasGaucheZ-0.3*rapidite;//-20
          JointureX=JointureX-0.15*rapidite;//-10
          JointureY=JointureY-0.5*rapidite;//-30
    }
    else
    {
          BrasDroitZ=BrasDroitZ+0.5*rapidite;
          BrasDroitY=BrasDroitY-0.15*rapidite;
          BrasGaucheY=BrasGaucheY-1*rapidite;
          BrasGaucheZ=BrasGaucheZ+0.3*rapidite;
          AvantBrasGaucheY=AvantBrasGaucheY-1.5*rapidite;
          AvantBrasGaucheZ=AvantBrasGaucheZ+0.3*rapidite;
          JointureX=JointureX+0.15*rapidite;
          JointureY=JointureY+0.5*rapidite;
    }
    if (AvantBrasGaucheY>120) DABB = false;
    if (AvantBrasGaucheY<0) DABB= true;

    glutPostRedisplay();
    }
}
void TeteCube(int c)//nombre de points, Hauteur d'une face, rayon, pas
{
    glEnable(GL_TEXTURE_2D);

    unsigned char* image = loadJpegImage("./visage.jpg",512,512);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE,image);


        glBegin(GL_POLYGON);
            glColor3f(1,1,1);
            glTexCoord2f(1,1);    glVertex3f(0,0,c);
            glTexCoord2f(0,1);    glVertex3f(c,0,c);
            glTexCoord2f(0,0);    glVertex3f(c,c,c);
            glTexCoord2f(1,0);    glVertex3f(0,c,c);
        glEnd();

    glDisable(GL_TEXTURE_2D);//SINON BUG SUR LES glColor!

    glEnable(GL_TEXTURE_2D);
    unsigned char* image2 = loadJpegImage("./face.jpg",512,512);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE,image2);

        glBegin(GL_POLYGON);
            glColor3f(1,1,1);
            glTexCoord2f(1,1);    glVertex3f(0,0,0);
            glTexCoord2f(0,1);    glVertex3f(c,0,0);
            glTexCoord2f(0,0);    glVertex3f(c,c,0);
            glTexCoord2f(1,0);    glVertex3f(0,c,0);
        glEnd();

        glBegin(GL_POLYGON);
            glColor3f(1,1,1);
            glTexCoord2f(1,1);    glVertex3f(c,0,0);
            glTexCoord2f(0,1);    glVertex3f(c,0,c);
            glTexCoord2f(0,0);    glVertex3f(c,c,c);
            glTexCoord2f(1,0);    glVertex3f(c,c,0);
        glEnd();
        glBegin(GL_POLYGON);
            glColor3f(1,1,1);
            glTexCoord2f(1,1);    glVertex3f(0,0,0);
            glTexCoord2f(0,1);    glVertex3f(0,0,c);
            glTexCoord2f(0,0);    glVertex3f(0,c,c);
            glTexCoord2f(1,0);    glVertex3f(0,c,0);
        glEnd();
        //faces sommet/base
        glBegin(GL_POLYGON);
            glColor3f(1,1,1);
            glTexCoord2f(0,0);    glVertex3f(0,0,0);
            glTexCoord2f(1,0);    glVertex3f(c,0,0);
            glTexCoord2f(1,1);    glVertex3f(c,0,c);
            glTexCoord2f(0,1);    glVertex3f(0,0,c);
        glEnd();
        glBegin(GL_POLYGON);
            glColor3f(1,1,1);
            glTexCoord2f(0,0);    glVertex3f(0,c,0);
            glTexCoord2f(1,0);    glVertex3f(c,c,0);
            glTexCoord2f(1,1);    glVertex3f(c,c,c);
            glTexCoord2f(0,1);    glVertex3f(0,c,c);
        glEnd();

  glDisable(GL_TEXTURE_2D);//SINON BUG SUR LES glColor!
}

   //la TLOUUUWELLE

void Helicoide(int n ,double H ,double r)//nombre de points, Hauteur d'une face, rayon, pas
{
glEnable(GL_TEXTURE_2D);

    unsigned char* image = loadJpegImage("./loicomo.jpg",200,200);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,200,200,0,GL_RGB,GL_UNSIGNED_BYTE,image);


double cylindreXp[n];
double cylindreYp[n];
double cylindreZp[n];

double cylindreXg[n];
double cylindreYg[n];
double cylindreZg[n];


int i;

    for(i=0; i<n ;i++)//petit
    {
        cylindreXp[i] = double(r * cos(i*0.5));
        cylindreYp[i] = double(r * sin(i*0.5));
        cylindreZp[i] = double(H * i);

        cylindreXg[i] = 0;
        cylindreYg[i] = 0;
        cylindreZg[i] = H * i;
    }

    for(i = 0; i<n-1 ; i++){
        glBegin(GL_POLYGON);
            glColor3f(1,1,1);
            glTexCoord2f((double)i / (n+1),0.0);    glVertex3f(cylindreXp[i], cylindreYp[i], cylindreZp[i]);
            glTexCoord2f((double)i / (n+1),0.0);    glVertex3f(cylindreXg[i], cylindreYg[i], cylindreZg[i]);
            glTexCoord2f((double)(i+1) / (n+1),0.0);    glVertex3f(cylindreXg[i+1], cylindreYg[i+1], cylindreZg[i+1]);
            glTexCoord2f((double)(i+1) / (n+1),0.0);    glVertex3f(cylindreXp[i+1], cylindreYp[i+1], cylindreZp[i+1]);
        glEnd();
        }
  glDisable(GL_TEXTURE_2D);//SINON BUG SUR LES glColor!
}
/*
    //fin la tlouwelle*/


void affichage()
{

  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);

  glLoadIdentity();
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-camera,camera,-camera,camera,-camera,camera);
//////////////////lumiere/////////////////////////////

glPushMatrix();
  glRotatef(Rx,1,0,0);
  glRotatef(Ry,0,1,0);

 	glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST); 	// Active le test de profondeur, c'est-à-dire que si un point d'une face se situe derrière une autre face, il n'est pas dessiné
  	glEnable(GL_LIGHTING); 	// Active l'éclairage
    glMatrixMode(GL_MODELVIEW);

 	glEnable(GL_LIGHT0);
    float LightPos[4]={0,15,-3,1};
    glLightfv(GL_LIGHT0,GL_POSITION,LightPos);// positionnement de la lumière
    float lightSpecular[4]={1,1,1,1};
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);// couleur et type de lumière

    glEnable(GL_LIGHT1);
    float lightDif1[4] = {0,0,1,1};
    float lightAmb1[4] = {0.5,0.5,0.5,1};
    float spotDir1[3] = {-1,-3,-1};
    float lightPos1[4] = {0,12,6,1};
    glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDif1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir1);
    glLighti(GL_LIGHT1, GL_SPOT_CUTOFF, 70);

 	glEnable(GL_LIGHT2);
    float LightPos2[4]={0,15,3,1};
    glLightfv(GL_LIGHT2,GL_POSITION,LightPos2);// positionnement de la lumière
    float lightDiffuse[4]={1,0,1,1};
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse);// couleur et type de lumière


//////////////////////////////////////////////////////
    drawFloor(8);


    glPushMatrix();//animation  jambe + tibia + pied gauche
        glTranslatef(0.0,5.125,0.0);
        glRotatef(JTPDroite,1,0,0);
        glTranslatef(0.0,-5.125,0.0);

        glPushMatrix();// animation tibia + pied gauche

            glTranslatef(0.0,2.75,0.0);
            glRotatef(TPDroite,1,0,0);
            glTranslatef(0.0,-2.75,0.0);

//////////////JAMBE GAUCHE/////////////////////////////
            glPushMatrix();//pied gauche
                glPushMatrix();
                    glColor3f(0.7,0,0);
                    glTranslatef(0.0,0.125,0.0);
                    glScalef(1,0.25,1.5);
                    glutSolidCube(1);
                    glPopMatrix();

                glPushMatrix();
                    glColor3f(0.9,0,0);
                    glTranslatef(0,0.5,-0.75);
                    glutSolidCylinder(0.25,1.5,30,30);
                glPopMatrix();
            glPopMatrix();

            glPushMatrix();//tibia gauche
                glPushMatrix();
                    glColor3f(0.7,0,0);
                    glTranslatef(0.0,1.625,0.0);
                    glScalef(0.75,2,0.75);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glColor3f(0.9,0,0);
                    glTranslatef(-0.5,2.75,0);
                    glRotatef(90,0,1,0);
                    glutSolidCylinder(0.25,1,30,30);
                glPopMatrix();
            glPopMatrix();

        glPopMatrix();//fin animation de tibia + pied

            glPushMatrix();//jambe gauche
                glPushMatrix();
                    glColor3f(0.7,0,0);
                    glTranslatef(0.0,3.95,0.0);
                    glScalef(0.75,2,0.75);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();
                    glColor3f(0.9,0,0);
                    glTranslatef(-0.5,5.125,0);
                    glRotatef(90,0,1,0);
                    glutSolidCylinder(0.25,1,30,30);
                glPopMatrix();
            glPopMatrix();

    glPopMatrix();// fin animation jambe gauche

//////////////////////////////////////////////////////////////

    glPushMatrix();//animation jambe + tibia + pied droit
        glTranslatef(0.0,5.125,0.0);
        glRotatef(JTPGauche,1,0,0);
        glTranslatef(0.0,-5.125,0.0);


//////////////////JAMBE DROITE/////////////////////////
            glPushMatrix();
                glTranslatef(2,0,0);


                glPushMatrix();// animation tibia + pied droit
                    glTranslatef(0.0,2.75,0.0);
                    glRotatef(TPGauche,1,0,0);
                    glTranslatef(0.0,-2.75,0.0);
                    glPushMatrix();
                        glPushMatrix();  //pied
                            glColor3f(0.7,0,0);
                            glTranslatef(0.0,0.125,0.0);
                            glScalef(1,0.25,1.5);
                            glutSolidCube(1);
                        glPopMatrix();

                        glPushMatrix();
                            glColor3f(0.9,0,0);
                            glTranslatef(0,0.5,-0.75);
                            glutSolidCylinder(0.25,1.5,30,30);
                        glPopMatrix();
                    glPopMatrix();

                    glPushMatrix();//tibia
                        glPushMatrix();
                            glColor3f(0.7,0,0);
                            glTranslatef(0.0,1.625,0.0);
                            glScalef(0.75,2,0.75);
                            glutSolidCube(1);
                        glPopMatrix();

                        glPushMatrix();
                            glColor3f(0.9,0,0);
                            glTranslatef(-0.5,2.75,0);
                            glRotatef(90,0,1,0);
                            glutSolidCylinder(0.25,1,30,30);
                        glPopMatrix();
                    glPopMatrix();
            glPopMatrix();
                    glPushMatrix();//jambe
                        glPushMatrix();
                            glColor3f(0.7,0,0);
                            glTranslatef(0.0,3.95,0.0);
                            glScalef(0.75,2,0.75);
                            glutSolidCube(1);
                        glPopMatrix();

                        glPushMatrix();
                            glColor3f(0.9,0,0);
                            glTranslatef(-0.5,5.125,0);
                            glRotatef(90,0,1,0);
                            glutSolidCylinder(0.25,1,30,30);
                        glPopMatrix();
                    glPopMatrix();

        glPopMatrix();//fin décallage de 2
    glPopMatrix();// fin animation jambe complete

////////////////////////////////////////////////////
///////////////////////////torse////////////////////


            glPushMatrix();//Bas Ventre
                    glPushMatrix();
                        glColor3f(0.7,0,0);
                        glTranslatef(1,5.5,0.0);
                        glScalef(4,0.5,2);
                        glutSolidCube(1);
                    glPopMatrix();
            glPopMatrix();

            glPushMatrix();//animation haut du corps

                glTranslatef(1,6.125,0.0);
                glRotatef(JointureY,0,1,0);
                glRotatef(JointureX,1,0,0);
                glTranslatef(-1,-6.125,0.0);

                glPushMatrix();// Jointure torse + bas ventre
                    glColor3f(0.9,0,0);
                    glTranslatef(1,6.125,0.0);
                    glutSolidSphere(0.9,20,20);
                glPopMatrix();

                glPushMatrix();//torse
                        glPushMatrix();
                            glColor3f(0.7,0,0);
                            glTranslatef(1,9,0.0);
                            glScalef(4.5,5,2.5);
                            glutSolidCube(1);
                        glPopMatrix();
                glPopMatrix();

                glPushMatrix();//Bras gauche complet animation

                    glTranslatef(-1,11,0.0);
                    glRotatef(BrasGaucheY,0,1,0);
                    glRotatef(BrasGaucheZ,0,0,1);
                    glTranslatef(1,-11,0.0);

                        glPushMatrix();//epaule gauche
                            glColor3f(0.7,1,0);
                            glTranslatef(-1,11,0.0);
                            glutSolidSphere(1,20,20);
                        glPopMatrix();

                        glPushMatrix();//biceps gauche
                            glColor3f(0.7,1,1);
                            glTranslatef(-3.75,11,0.0);
                            glScalef(2.5,0.65,0.65);
                            glutSolidSphere(1,20,20);
                        glPopMatrix();

                        glPushMatrix();//avant bras gauche + DRILL


                            glTranslatef(-6,11,0.0);

                            glRotatef(AvantBrasGaucheY+By,0,1,0);
                            glRotatef(AvantBrasGaucheZ+Bz,0,0,1);
                            glTranslatef(6,-11,0.0);

                            glPushMatrix();//coude gauche
                                glColor3f(0.7,0,1);
                                glTranslatef(-6,11,0.0);
                                glutSolidSphere(0.5,20,20);
                            glPopMatrix();

                            glPushMatrix();//avantbras gauche
                                glColor3f(0,0,1);
                                glTranslatef(-9,11,0.0);
                                glRotatef(90,0,1,0);
                                glutSolidCylinder(0.3,2.5,20,20);
                            glPopMatrix();

                            glPushMatrix();
                                glTranslatef(-11.5,11,0);
                                glRotatef(X,1,0,0);//Idle Rotation
                                glRotatef(90,0,1,0);
                                Helicoide(50,0.05,0.5);//nombre de points, Hauteur d'une face, rayon
                            glPopMatrix();

                        glPopMatrix();//FIN avant bras + main animation


                glPopMatrix();//FIN Bras droit complet animation

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////BRAS DROIT///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


                glPushMatrix();
                glTranslatef(2,0,0);
                glRotatef(180,0,1,0),
                glPushMatrix();//Bras droit complet animation

                    glTranslatef(-1,11,0.0);
                    glRotatef(BrasDroitY,0,1,0);
                    glRotatef(BrasDroitZ,0,0,1);
                    glTranslatef(1,-11,0.0);

                        glPushMatrix();//epaule gauche
                            glColor3f(0.7,1,0);
                            glTranslatef(-1,11,0.0);
                            glutSolidSphere(1,20,20);
                        glPopMatrix();

                        glPushMatrix();//biceps gauche
                            glColor3f(0.7,1,1);
                            glTranslatef(-3.75,11,0.0);
                            glScalef(2.5,0.65,0.65);
                            glutSolidSphere(1,20,20);
                        glPopMatrix();

                        glPushMatrix();//avant bras gauche + pioche


                            glTranslatef(-6,11,0.0);

                            glRotatef(AvantBrasDroitY,0,1,0);
                            glRotatef(AvantBrasDroitZ,0,0,1);
                            glTranslatef(6,-11,0.0);

                            glPushMatrix();//coude droite
                                glColor3f(0.7,0,1);
                                glTranslatef(-6,11,0.0);
                                glutSolidSphere(0.5,20,20);
                            glPopMatrix();

                            glPushMatrix();//avantbras droit
                                glColor3f(0,0,1);
                                glTranslatef(-9,11,0.0);
                                glRotatef(90,0,1,0);
                                glutSolidCylinder(0.3,2.5,20,20);
                            glPopMatrix();


                            glTranslatef(-9,11,0);
                            glRotatef(XPioche,1,0,0);/////////////////////////////////////IDLE
                            glTranslatef(9,-11,0);

                            glPushMatrix();//base du marteau
                                glColor3f(1,1,0);
                                glTranslatef(-9,9.5,0);
                                glScalef(0.7,0.3,0.7);
                                glutSolidCube(1);
                            glPopMatrix();

                            glPushMatrix();//manche du marteau
                                glColor3f(1,0,1);
                                glTranslatef(-9,9.5,0);
                                glRotatef(-90,1,0,0);
                                glutSolidCylinder(0.2,6,5,5);
                            glPopMatrix();

                            glPushMatrix();// partie centrale du dessus de la pioche
                                glColor3f(1,1,0.5);
                                glTranslatef(-9,15.6,0);
                                glScalef(1,0.6,5);
                                glutSolidCube(1);
                            glPopMatrix();

                            glPushMatrix();// partie laterale du dessus de la pioche
                                glColor3f(1,0.7,0);
                                glTranslatef(-9,15.5,-3.5);
                                glRotatef(90,0,1,0);
                                glScalef(3,0.5,1);
                                glutSolidTetrahedron();
                                glTranslatef(9,15.5,3.5);
                            glPopMatrix();

                            glPushMatrix();// partie laterale du dessus de la pioche
                                glColor3f(1,0.7,0);
                                glTranslatef(-9,15.5,3.5);
                                glRotatef(270,0,1,0);
                                glScalef(3,0.5,1);
                                glutSolidTetrahedron();
                                glTranslatef(9,15.5,3.5);
                            glPopMatrix();

                        glPopMatrix();//FIN avant bras + main animation
                    glPopMatrix();//FIN translate rotate


                    glPushMatrix();//cou
                        glColor3f(1,0.7,0);
                        glTranslatef(1,12.5,0);
                        glRotatef(90,1,0,0);
                        glutSolidCylinder(1,2,20,20);
                    glPopMatrix();

                    glPushMatrix();//cou
                        glTranslatef(1,12.5,0);
                        glutSolidSphere(1,20,20);
                    glPopMatrix();

                glPopMatrix();//tete + cou animation
                    glTranslatef(1,12.5,0.0);
                    glRotatef(TeteX,1,0,0);
                    glRotatef(TeteY,0,1,0);
                    glRotatef(TeteZ,0,0,1);
                    glTranslatef(-1,-12.5,0.0);
                    glPushMatrix();//tete
                        glTranslatef(-0.5,12.5,-1.5);
                        TeteCube(3);
                    glPopMatrix();
                    glPushMatrix();//antenne 1
                        glColor3f(1,0.7,0);
                        glTranslatef(1,17.5,0);
                        glRotatef(90,1,0,0);
                        glutSolidCylinder(0.25,2,10,10);
                    glPopMatrix();
                    glPushMatrix();//antenne 2
                        glColor3f(0.5,0.9,0);
                        glTranslatef(1,17.5,0);
                        glutSolidSphere(0.4,10,10);
                    glPopMatrix();

                glPopMatrix();//FIN tete + cou animation

///////////////////////////////////////////////////////////////

            glPopMatrix();//fin animation haut du corps


    ///////////////////////////////////////////////////////////////////
glPopMatrix();

  //On echange les buffers
  glutSwapBuffers();
}

void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
/////////JAMBE DROITE ANIMATION
/////////////////////////////////////////
/////////JAMBE GAUCHE ANIMATION

    case '('://Marche avant

        if(MarcheM) MarcheM=false;
        else MarcheM = true;
      break;

////////FIN JAMBE GAUCHE ANIMATION
//////////////////////////////////////
////////CORPS ANIMATION
    case 'i'://CORPS
      JointureX=JointureX+2;
      if(JointureX>30) JointureX=30;
      glutPostRedisplay();
      break;
      case 'I':
      JointureX=JointureX-2;
      if(JointureX<-30) JointureX=-30;
      glutPostRedisplay();
      break;

    case 'k'://CORPS
      JointureY=JointureY+2;
      if(JointureY>360) JointureY=0;
      glutPostRedisplay();
      break;
      case 'K':
      JointureY=JointureY-2;
      if(JointureY<-360) JointureY=0;
      glutPostRedisplay();
      break;
////////////////FIN CORPS
////////////////////////////////////////
//////////////////BRAS
      case 'p'://Bras droit Complet
      BrasDroitZ=BrasDroitZ-2;
      if(BrasDroitZ<-360) BrasDroitZ=0;
      glutPostRedisplay();
      break;
      case 'P'://Bras droit Complet
      BrasDroitZ=BrasDroitZ+2;
      if(BrasDroitZ>360) BrasDroitZ=0;
      glutPostRedisplay();
      break;
       case 'o'://Bras droit Complet
      BrasDroitY=BrasDroitY-2;
      if(BrasDroitY<-360) BrasDroitY=0;
      glutPostRedisplay();
      break;
       case 'O'://Bras droit Complet
      BrasDroitY=BrasDroitY+2;
      if(BrasDroitY>360) BrasDroitY=0;
      glutPostRedisplay();
      break;

      case 'l'://AVANT Bras droit
      AvantBrasDroitY=AvantBrasDroitY-2;
      if(AvantBrasDroitY<-360) AvantBrasDroitY=0;
      glutPostRedisplay();
      break;
      case 'L'://AVANT Bras droit
      AvantBrasDroitY=AvantBrasDroitY+2;
      if(AvantBrasDroitY>360) AvantBrasDroitY=0;
      glutPostRedisplay();
      break;
      case 'm'://AVANT Bras droit
      AvantBrasDroitZ=AvantBrasDroitZ-2;
      if(AvantBrasDroitZ<-360) AvantBrasDroitZ=0;
      glutPostRedisplay();
      break;
       case 'M':// AVANT Bras droit
      AvantBrasDroitZ=AvantBrasDroitZ+2;
      if(AvantBrasDroitZ>360) AvantBrasDroitZ=0;
      glutPostRedisplay();
      break;
/////////FIN BRAS DROIT
/////////////////////////////////////////////
/////////BRAS GAUCHE ANIMATION
      case 'y'://Bras Gauche Complet
      BrasGaucheZ=BrasGaucheZ-2;
      if(BrasGaucheZ<-360) BrasGaucheZ=0;
      glutPostRedisplay();
      break;
      case 'Y'://Bras Gauche Complet
      BrasGaucheZ=BrasGaucheZ+2;
      if(BrasGaucheZ>360) BrasGaucheZ=0;
      glutPostRedisplay();
      break;
       case 'u'://Bras Gauche Complet
      BrasGaucheY=BrasGaucheY+2;
      if(BrasGaucheY>360) BrasGaucheY=0;
      glutPostRedisplay();
      break;
       case 'U'://Bras Gauche Complet
      BrasGaucheY=BrasGaucheY-2;
      if(BrasGaucheY<-360) BrasGaucheY=0;
      glutPostRedisplay();
      break;

      case 'j'://AVANT Bras Gauche
      By=By+2;
      if(By>360)  By=0;
      glutPostRedisplay();
      break;
      case 'J'://AVANT Bras Gauche
      By=By-2;
      if(By<-360) By=0;
      glutPostRedisplay();
      break;

      case 'h'://AVANT Bras Gauche
      Bz=Bz-2;
      if(Bz<-360) Bz=0;
      glutPostRedisplay();
      break;
       case 'H':// AVANT Bras Gauche
      Bz=Bz+2;
      if(Bz>360) Bz=0;
      glutPostRedisplay();
      break;
/////////FIN BRAS GAUCHE ANIMATION
/////////////////////////////////////////////
/////////TETE ANIMATION
      case '4'://tete y
      TeteY=TeteY-2;
      if(TeteY<-360) TeteY=0;
      glutPostRedisplay();
      break;
       case '6':
      TeteY=TeteY+2;
      if(TeteY>360) TeteY=0;
      glutPostRedisplay();
      break;

      case '9'://tete z
      TeteZ=TeteZ-2;
      if(TeteZ<-20)TeteZ=-20;
      glutPostRedisplay();
      break;
       case '7':
      TeteZ=TeteZ+2;
      if(TeteZ>20)TeteZ=20;
      glutPostRedisplay();
      break;

      case '8'://tete x
      TeteX=TeteX-2;
      if(TeteX<-20)TeteX=-20;
      glutPostRedisplay();
      break;
       case '5':
      TeteX=TeteX+2;
      if(TeteX>20)TeteX=20;
      glutPostRedisplay();
      break;
/////////FIN TETE ANIMATION

    case '&'://DAB

        if(DABM) DABM=false;
        else DABM= true;
      break;

      break;

    case 'r'://RESET

         JTPDroite=0;TPDroite=0;
         JTPGauche=0;TPGauche=0;
         JointureY=0;JointureX=0;
         BrasDroitY=0;BrasDroitZ=0;
         AvantBrasDroitY=0;AvantBrasDroitZ=0;
         BrasGaucheY=0;BrasGaucheZ=0;
         AvantBrasGaucheY=0;AvantBrasGaucheZ=0;
         TeteX=0;TeteY=0;TeteZ=0,By=0,Bz=0;
         camera=20;

        glutPostRedisplay();
      break;


    case '"'://PIOCHE ON / OFF

        if(PiocheM) PiocheM=false;
        else PiocheM= true;
      break;


      case 'z'://ZOOM +

        if (camera>1)camera=camera-1;
      break;
      case 'Z'://ZOOM -

        camera=camera+1;
      break;


    case 'q' : /*la touche 'q' permet de quitter le programme */
        exit(0);
    }
}

void touche(int touche,int x,int y)
{
  switch (touche)
    {
       case GLUT_KEY_UP:
      Rx=Rx-2;
      glutPostRedisplay();
      break;
       case GLUT_KEY_DOWN:
      Rx=Rx+2;
      glutPostRedisplay();
      break;
       case GLUT_KEY_LEFT:
      Ry=Ry-2;
      glutPostRedisplay();
      break;
       case GLUT_KEY_RIGHT:
      Ry=Ry+2;
      glutPostRedisplay();
      break;
      }
}

void souris(int bouton, int etat,int x,int y)
{
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_DOWN)
  {
    presse = 1;
    xold = x;
    yold=y;
  }
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_UP)
    presse=0;
}

void sourismouv(int x,int y)
  {
    if (presse)
    {
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay();
    }

    xold=x;
    yold=y;
  }


unsigned char *loadJpegImage(char *fichier, int limg, int himg)
{
  unsigned char image[limg*himg*3];
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *file;
  unsigned char *ligne;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
#ifdef __WIN32
  if (fopen_s(&file,fichier,"rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#elif __GNUC__
  if ((file = fopen(fichier,"rb")) == 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#endif
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);


  if ((cinfo.image_width!=limg)||(cinfo.image_height!=himg)) {
    fprintf(stdout,"Erreur : l'image n'a pas la bonne taille.\n");
    exit(1);
  }
  if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
    fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
    exit(1);
  }



  jpeg_start_decompress(&cinfo);
  ligne=image;
  while (cinfo.output_scanline<cinfo.output_height)
    {
      ligne=image+3*limg*cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo,&ligne,1);
    }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  return image;
}
