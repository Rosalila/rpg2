#include <iostream>
#include <irrlicht.h>
using namespace irr;
using namespace std;
#include "MyEventReceiver.h"


class Grafico
{
    public:
    IrrlichtDevice *device;
    video::IVideoDriver* driver;
    gui::IGUIFont* font;
    Grafico(MyEventReceiver* e)
    {
        video::E_DRIVER_TYPE driverType;
        driverType = video::EDT_OPENGL;
        // create device
        device = createDevice(driverType,core::dimension2d<u32>(1024, 600), true, true, false, false, e);
        if (device == 0)
            return; // could not create selected driver.
        device->setWindowCaption(L"RPG");
        driver = device->getVideoDriver();
        //init fonts
        font = device->getGUIEnvironment()->getFont("recursos/font.bmp");
    }

	void draw2DImage
	(
	             irr::video::ITexture* texture,
				 irr::core::dimension2d<irr::f32> size,
				 irr::core::rect<irr::f32> sourceRect,
				 irr::core::position2d<irr::f32> position,
				 irr::core::position2d<irr::f32> rotationPoint,
				 irr::f32 rotation , irr::core::vector2df scale,
				 bool useAlphaChannel,
				 irr::video::SColor color ,
				 bool flipHorizontally,
				 bool flipVertically 	)
	{

   if(!texture)
	   return;

   irr::video::SMaterial material;

   irr::core::vector2df corner[4];

   corner[0] = irr::core::vector2df(position.X,position.Y);
   corner[1] = irr::core::vector2df(position.X+size.Width*scale.X,position.Y);
   corner[2] = irr::core::vector2df(position.X,position.Y+size.Height*scale.Y);
   corner[3] = irr::core::vector2df(position.X+size.Width*scale.X,position.Y+size.Height*scale.Y);


   if (rotation != 0.0f)
      for (int x = 0; x < 4; x++)
         corner[x].rotateBy(rotation,irr::core::vector2df(rotationPoint.X, rotationPoint.Y));


   //TODO: ver como mejorar esto
   irr::core::vector2df uvCorner[4];

   if(!flipVertically && !flipHorizontally)
   {
	   uvCorner[0] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.UpperLeftCorner.Y);
	   uvCorner[1] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.UpperLeftCorner.Y);
	   uvCorner[2] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.LowerRightCorner.Y);
	   uvCorner[3] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.LowerRightCorner.Y);
   }
   else if(!flipVertically && flipHorizontally)
   {
	   uvCorner[1] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.UpperLeftCorner.Y);
	   uvCorner[0] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.UpperLeftCorner.Y);
	   uvCorner[3] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.LowerRightCorner.Y);
	   uvCorner[2] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.LowerRightCorner.Y);
   }
   else if(flipVertically && !flipHorizontally)
   {
	   uvCorner[2] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.UpperLeftCorner.Y);
	   uvCorner[3] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.UpperLeftCorner.Y);
	   uvCorner[0] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.LowerRightCorner.Y);
	   uvCorner[1] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.LowerRightCorner.Y);
   }
   else
   {
	   uvCorner[3] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.UpperLeftCorner.Y);
	   uvCorner[2] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.UpperLeftCorner.Y);
	   uvCorner[1] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.LowerRightCorner.Y);
	   uvCorner[0] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.LowerRightCorner.Y);
   }


   for (int x = 0; x < 4; x++) {
      float uvX = uvCorner[x].X/(float)texture->getSize().Width;
      float uvY = uvCorner[x].Y/(float)texture->getSize().Height;
      uvCorner[x] = irr::core::vector2df(uvX,uvY);
   }


   irr::video::S3DVertex vertices[4];
   irr::u16 indices[6] = { 0, 1, 2, 3 ,2 ,1 };


   float screenWidth = (float)driver->getScreenSize().Width;
   float screenHeight = (float)driver->getScreenSize().Height;
   for (int x = 0; x < 4; x++) {
      float screenPosX = ((corner[x].X/screenWidth)-0.5f)*2.0f;
      float screenPosY = ((corner[x].Y/screenHeight)-0.5f)*-2.0f;
      vertices[x].Pos = irr::core::vector3df(screenPosX,screenPosY,1);
      vertices[x].TCoords = uvCorner[x];
      vertices[x].Color = color;
   }
   material.Lighting = false;
   material.ZWriteEnable = false;
   material.TextureLayer[0].Texture = texture;

   if (useAlphaChannel)
      material.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
   else
      material.MaterialType = irr::video::EMT_SOLID;


   driver->setMaterial(material);
   driver->drawIndexedTriangleList(&vertices[0],4,&indices[0],2);
   driver->drawPixel(100,100,color);
}
};

class Combate{

public:
Grafico* grafico;
Combate(Grafico* gr){
    grafico=gr;
}
void fondo(){
    video::ITexture* back=grafico->driver->getTexture("recursos/fondo/valle.jpg");
    grafico->draw2DImage
    (
        back,
        irr::core::dimension2d<irr::f32> (1024,600),
        irr::core::rect<irr::f32>(0,0,1024,600),
        irr::core::position2d<irr::f32>(0,0),
        irr::core::position2d<irr::f32>(0,0),
        irr::f32(0), irr::core::vector2df (1,1),
        true,
        irr::video::SColor(255,255,255,255),
        false,
        false);
}

void indicador(int posY){
video::ITexture* i1=grafico->driver->getTexture("recursos/m1.png");
            grafico->draw2DImage
            (
                i1,
                irr::core::dimension2d<irr::f32> (16,15),
                irr::core::rect<irr::f32>(0,0,16,15),
                irr::core::position2d<irr::f32>(60,posY),
                irr::core::position2d<irr::f32>(0,0),
                irr::f32(0), irr::core::vector2df (1,1),
                true,
                irr::video::SColor(255,255,255,255),
                true,
                false);
}

void submenu(int opcion){

    if(opcion==2){
        grafico->driver->draw2DRectangle(video::SColor(255,128,0,255),core::rect<s32>(200,315,370,515));

        for(int i=315;i<515;i+=40)
        grafico->font->draw(L"Pelear",core::rect<s32>(230,i,245,i+15),video::SColor(255,255,255,255));

        grafico->font->draw(L"Pelear",core::rect<s32>(305,315,320,330),video::SColor(255,255,255,255));
    }

    if(opcion==3){
        grafico->driver->draw2DRectangle(video::SColor(255,128,0,255),core::rect<s32>(200,315,300,415));
    }
}

bool render(int posInd)
    {
		if (grafico->device->isWindowActive())
		{
		    grafico->driver->beginScene(true, true, video::SColor(255,255,255,255));
		    //fondo
            fondo();

		    //Dibujar rectangulo
		    grafico->driver->draw2DRectangle(video::SColor(255,128,255,255),core::rect<s32>(0,300,1024,600));
		    grafico->driver->draw2DRectangle(video::SColor(255,255,255,255),core::rect<s32>(512,315,550,585));
		    //Dibujar texto
            grafico->font->draw(L"Pelear",core::rect<s32>(100,350,200,400),video::SColor(255,255,255,255));
            grafico->font->draw(L"Magia",core::rect<s32>(100,400,200,450),video::SColor(255,255,255,255));
            grafico->font->draw(L"Item",core::rect<s32>(100,450,200,500),video::SColor(255,255,255,255));
            //Nombre y HP
            grafico->font->draw(L"Maverikku             HP 2000/2000",core::rect<s32>(650,350,600,400),video::SColor(255,255,255,255));
            //Cargar y dinujar una imagen

            indicador(posInd);
            submenu(2);
			grafico->driver->endScene();
		}
		return grafico->device->run() && grafico->driver;
    }

};

int main()
{
    MyEventReceiver* evento=new MyEventReceiver();
    Grafico* grafico=new Grafico(evento);
    Combate* x=new Combate(grafico);
    int posInd=350;
    int op=1;
    bool teclaU=true;
    for(;;)
    {

    if (evento->IsKeyDown(irr::KEY_DOWN) && posInd<450 && teclaU)
    {
        posInd+=50;
        op+=1;
        teclaU=false;
    }
    else if (evento->IsKeyDown(irr::KEY_DOWN) && posInd>=450 && teclaU)
    {
        posInd=350;
        op=1;
        teclaU=false;
    }
    if(!evento->IsKeyDown(irr::KEY_DOWN))
        teclaU=true;

    x->render(posInd);
    if(evento->IsKeyDown(irr::KEY_KEY_Q))
        exit(0);
    }
    cout << "Hello world!" << endl;
    return 0;
}
