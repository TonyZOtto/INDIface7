
## INDIface7
# Installing

## Offline Installer

### Create Package Directory

1. Create `/code/repo/INDIface7/install/config` 
      and `/code/temp/install/packages`
2. Within `packages` create:
  * `com.EclipseIR.root`
    * LEGAL.txt
  * `com.EclipseIR.root.If7ODEval`
  * `com.EclipseIR.root.Samples.Sample1`
  * `com.EclipseIR.root.Samples.Skyplex1`
  * Within each of these create `data` and `meta`

3. In `config` create `config.xml`
```
<?xml version="1.0" encoding="UTF-8"?>
<Installer>
    <Name>INDIface7 ObjDet Evaluation</Name>
    <Version>7.90.3</Version>
    <Title>INDIface7 Installer</Title>
    <Publisher>Eclipse Identity Recognition Corporation</Publisher>
    <StartMenuDir>EclipseIR</StartMenuDir>
    <TargetDir>/EclipseIR/bin</TargetDir>
</Installer>
```
4. In `\code\repo\INDIface7\install\packages\com.EclipseIR.If7ODEval\meta`
	create `package.xml`
```	
<?xml version="1.0" encoding="UTF-8"?>
<Package>
    <DisplayName>INDIface7 ObjDet Evaluation</DisplayName>
    <Description>Install the special engine.</Description>
    <Version>7.9.3</Version>
    <ReleaseDate>2010-03-05</ReleaseDate>
    <Licenses>
        <License name="Company Private" file="" />
    </Licenses>
    <Default>true</Default>
    <Script></Script>
    <UserInterfaces>
        <UserInterface>page.ui</UserInterface>
    </UserInterfaces>
</Package>
```

5. Copy to `\code\repo\INDIface7\install\packages\com.EclipseIR.root.If7ODEval\data`
  
  
  * `If7Engine.exe` as `EclipseIR.If7ODEval.exe`
  
  
  * Necessary `libopencv_*455.dll`
  * Necessary Qt6 libraries plus dependendies:
    * libgcc*.dll 
	* libstdc++*.dll
	* libwinpthread*.dll
  * `detectors` Directory
  * `EvalIn` Directory
    * with a sample or two
	
1. Make sure `/code/bin/install/RelWin64-Qt6CV4-v7.90C` directory exists.
	
6. In CMD box:
  * Run >`\Qt\6.10.0\mingw_64\bin\windeployqt --no-translations \code\bin\RelWin64-Qt6CV4-v7.90C`
  * Run >`xcopy \code\bin\RelWin64-Qt6CV4-v7.90C  \code\repo\INDIface7\install\packages\EclipseIR.IF7ODEval\data /S`
  * Run >`rmdir -F \code\repo\INDIface7\install\packages\EclipseIR.IF7ODEval\data\EvalIn /s /q`
  * Run >`rmdir -F \code\repo\INDIface7\install\packages\EclipseIR.IF7ODEval\data\EvalOut /s /q`
  cd /code/repo/INDIface7/install/
  * Run >`\Qt\Tools\QtInstallerFramework\4.8\bin\binarycreator  -t \Qt\Tools\QtInstallerFramework\4.8\bin\installerbase.exe -p packages -c config\config.xml `
  
  


