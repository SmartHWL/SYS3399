rd /s /q .git
move .\Objects\GeneralMotorCtrl.sct .\GeneralMotorCtrl.sct
rd /s /q Objects
mkdir Objects
move .\GeneralMotorCtrl.sct .\Objects\GeneralMotorCtrlFix.sct
rd /s /q silan_only
rd /s /q SourceInsight
del .gitignore
del *.bak
del *uvgui*
del *.dep