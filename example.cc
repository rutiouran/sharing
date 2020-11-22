#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

int main(int argc,char **argv)
{
	G4UIExecutive *ui = 0;		//检测交互模式（如果没有参数）并定义 UI 会话
	if (argc == 1)
	{
		ui = new G4UIExecutive(argc, argv);
	}


	#ifdef G4MULTITHREADED		//选择：选择一个不同的随机的引擎
	G4MTRunManger *runmanager = new G4MTRRunManager;	//G4Random::setTheEngine(new CLHEP::MTwistEngine);
	#else
	G4RunManager *runmanager = new G4RunManager;	//构造默认的运行管理器
	#endif

	runmanager->SetUserInitialization(new DetectorConstruction());		//设置强制性初始化类


	G4VModularPhysicsList *physicsList = new QBBC;		//物理清单
	physicsList->SetVerboseLevel(1);
	runmanager->SetUserInitialization(physicsList);


	runmanager->SetUserInitialization(new ActionInitialization());		//用户行为初始化


	G4VisManager *visManager = new G4VisExecutive;		//初始形象化
	visManager->Initialize();


	G4UImanager *UImanager = G4UImanager::GetUIpointer();		//获取指向用户界面管理器的指针


	if(!ui)		//加工宏指令或者开始 UI 会话
	{
		G4String command = "/control/execute ";		//处理模式
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
	else
	{
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		ui->SessionStart();
    }
}

