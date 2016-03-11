#include <pspkernel.h>
#include <pspmodulemgr.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspthreadman.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"
#include "pspcommon.h"

/* This is a rebuild HB of MasterBoy by davis.
	The Original version is a 2.02 this is a more stable now XD
*/

PSP_MODULE_INFO("MasterBoy", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-2048);


/* Exit callback */
/*int exit_callback(void)
{
//	SmsTerm();

//	adhocTerm();

	sceKernelExitGame();

	return 0;
}*/

/* Callback thread */
/*int CallbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}*/

/* Sets up the callback thread and returns its thread id */
/*int SetupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}*/

/*int (*oldPowerCallback)(int, int, void*)=NULL;

int myPowerCallback(int unknown, int pwrflags,void *common){
	int cbid;
	if ((pwrflags & PSP_POWER_CB_POWER_SWITCH))		{
		if (menuConfig.file.sramAutosave)
			machine_manage_sram(SRAM_SAVE, 0);
	}
	if (oldPowerCallback)
		return oldPowerCallback(unknown, pwrflags, common);
}*/
void InitConfig();
void MenuOptionsConfigure(int moment);
void ControlsUpdate(void);
void gbe_refresh();
void SmsTerm(void);

int main(int argc, char *argp[]){
	// Init More Stuff...
	
	// Initialize Osl
	oslInit(3); // Based on Onelua xD
	
	// Init Crls Functions..
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	
	// Init Configs Stuff.
	InitConfig();
	memcpy(menuConfigDefault, &menuConfig, sizeof(menuConfig)); //Sauvegarde pour plus tard
	
	// Init Stuff GU "Video"
	oslInitGfx(OSL_PF_8888, 1);// OSL_PF_5650 Aqui estaba esto.. Cambiado a 8888	
	oslInitConsole();
	oslStartDrawing();
	VideoGuScreenClear();// Check If really its vital
	/* Añadido
	oslIntraFontInit(INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	OSL_FONT * oneFont = oslLoadFontFile("flash0:/font/jpn0.pgf");
	oslIntraFontSetStyle(oneFont, 0.7, RGB(255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(oneFont);
	*/
	//Main screen: in VRAM for speed
	Screen.scrBuffer = oslGetUncachedPtr(oslVramMgrAllocBlock(SCR_BUFFER_SIZE));
	Screen.pal = oslGetUncachedPtr(oslVramMgrAllocBlock(256*2));
	//Sub screen: in RAM for mem sparing
	ScreenTemp.scrBuffer = oslGetUncachedPtr(malloc(SCR_BUFFER_SIZE));
	ScreenTemp.pal = NULL;
	bitmap.data   = (unsigned char*)Screen.scrBuffer;
	/* Term of stuff gu */
	
	// Init More Vars..
	u32 nFrame, nDrawnFrames, skip=0, framerate;
	int nbRenderedFramesPerSec, nbVirtualFramesPerSec = 0, lastVCount = 0;
	int frameskip = 0;

	memset(&bitmap, 0, sizeof(bitmap_t));
	bitmap.width  = 256;
	bitmap.height = 192;
	bitmap.depth  = menuConfig.video.renderDepth;
	bitmap.pitch  = 256 * menuConfig.video.renderDepth / 8;

	menuConfig.file.filename[0] = '\0';

	sms.save = 1;

	// Display the menu to load up the ROM
	//	displayMenu();
	menuIsInGame = 0;

	//menuPlusShowMenu();
	MenuPlusAction(MA_LOADROM, "ms0:/Poke_Oro.gbc");

	//Bidouille pour détecter les choses qui ont changé
	MenuOptionsConfigure(-1);
	MenuOptionsConfigure(0);

	VideoGuScreenClear();

	//Système
	SmsInit();
	nFrame = nDrawnFrames = 0;
	menuConfig.video.turbo = menuConfig.video.pause = 0;
	nbRenderedFramesPerSec = 0;
	gblFramerate = gblVirtualFramerate = 0;
	menuUpdateRender = 0;

	//Système
	if (gblMachineType == EM_SMS)
		system_poweron();

	while(1){ // Principal Cycle
		//Pal support
		if (menuConfig.video.country == 1)
			framerate = 50;
		else
			framerate = 60;
		oslSetFramerate(framerate);

		if (menuUpdateRender)
			nDrawnFrames = 0, menuUpdateRender = 0;

		if (gblMachineType == EM_SMS)
			ControlsUpdate();
		else if (gblMachineType == EM_GBC){
			gbe_refresh();
			gbe_updatePad();
		}

		if (osl_quit)
			break;

		//Framerate
		if (menuConfig.video.cpuTime == 2){
			int vcount = osl_vblCount;
			if (!skip)
				nbRenderedFramesPerSec++;
			nbVirtualFramesPerSec++;
			if (vcount / framerate > lastVCount / framerate)			{
				gblFramerate = nbRenderedFramesPerSec;
				gblVirtualFramerate = nbVirtualFramesPerSec;
				nbRenderedFramesPerSec = 0;
				nbVirtualFramesPerSec = 0;
			}
			lastVCount = vcount;
		}

		if (!menuConfig.video.pause){
			if (gblMachineType == EM_SMS){
				system_frame(skip);
				if (menuConfig.sound.turboSound /*== 2*/ || !menuConfig.video.turbo){
					//On peut commencer à jouer le son: tout est initialisé (sinon il faut attendre)
					soundPause = 0;
					if (!menuConfig.sound.perfectSynchro){
						SoundUpdate();
					}
				}
			}
			else if (gblMachineType == EM_GBC){
				//Run one GB frame
				gb_doFrame(skip);
				soundPause = 0;
				if (menuConfig.sound.turboSound /*== 2*/ || !menuConfig.video.turbo)		{
					soundPause = 0;
					if (!menuConfig.sound.perfectSynchro)			{
						snd_render_orig(snd.output, snd.sample_count);
						SoundUpdate();
					}
				}
			}
		}

		if (!skip && menuConfig.video.syncMode == 0){
			//if (gblMachineType == EM_SMS)
			VideoGuUpdate(nDrawnFrames, menuConfig.video.render);
			frameReady = 1;
			nDrawnFrames++;
		}
		oslDrawString(10,10,"Mod By Davis");
		//vsyncAdd = 0;
		//redo:
		if (menuConfig.video.turbo){
			if (!skip){
				oslEndDrawing();
				oslSwapBuffers();
			}
			skip = nFrame % (menuConfig.video.turboSkip + 1);
		}
		else{
			int vsync = 4 /*+ vsyncAdd*/;
			if (menuConfig.video.vsync == 2)
				vsync |= 1;
			/*else if (menuConfig.video.vsync == 1)
				vsync |= 8;*/

			/*if (autoFskip && !skip)
				tmp = oslMeanBenchmarkTestEx(OSL_BENCH_END, 0);*/

			skip = oslSyncFrameEx(oslMinMax(frameskip+1, menuConfig.video.frameskip, menuConfig.video.frameskipMax), menuConfig.video.frameskipMax, vsync);

			/*if (autoFskip && !skip)		{
				//Store the CPU usage (in percent)
				cpuTime = tmp * 6 / 1000;
				oslBenchmarkTest(OSL_BENCH_START);
			}*/
		}

		/*if (autoFskip){
			if (!skip){
				cpuTime = oslMeanBenchmarkTestEx(OSL_BENCH_GET_LAST, 0) * 6 / 1000;
				oslMeanBenchmarkTestEx(OSL_BENCH_START, 0);
				targetFrameskip = skippedFrames;
				skippedFrames = 0;
			}
			else{
				skippedFrames++;
			}
		}*/
		if (!skip && menuConfig.video.syncMode == 1)			{
			//if (gblMachineType == EM_SMS)
			VideoGuUpdate(nDrawnFrames, menuConfig.video.render);
			frameReady = 1;
			nDrawnFrames++;
		}

		nFrame++;
		//Pal
		/*if (menuConfig.video.country == 1 && nFrame % 6 == 0)		{
			if (!skip)
				vsyncAdd = 16;
			goto redo;
		}*/
		//In game, menu not currently shown - WARNING: DONT MOVE THIS, THE MESSAGEBOX CODE RELIES ON menuIsInGame == 2 TO KNOW IF THE GAME IS CURRENTLY RUNNING (or menuIsInGame == 1 = menu is shown)
		menuIsInGame = 2;

		if (menuDisplaySpecialMessage)
			MenuSpecialMessageDisplay();
	} // End Of While
	SmsTerm();
	//SmsEmulate();
	// End Emulate
	SaveMyPlacesFile();
	sceKernelExitGame();

    return 0;
}
