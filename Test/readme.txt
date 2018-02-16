测试环境结构如下

|\.
|--bin
   |--GraphBuild.exe  //生成测试数据的应用程序
   |--FindRoad.exe    //搜寻结果的程序

|--conf
   |--GraphInfo.txt   //配置测试数据的生成需求

|--data               //data目录，存放生成文件        
   |--topo.csv        
   |--demand.csv
   |--result.csv      //参考答案
   |--myresult.csv    //使用寻路算法搜得的测试答案

|--script
  |--BuildGraph.bat   //启动测试数据的生成
  |--FindRoad.bat     //启动算法搜路

|--log  
   |--testlog.log     //寻路日志


使用说明：
在script目录中进行测试即可。
1.先在conf目录下配置GraphInfo.conf文件。
2.接着双击script下的BuildGraph.bat，即可生成图文件topo和需求文件demand，以及参考答案result。 如果显示生成999999说明生成失败，因为有bug没处理好。重新启动一次。

3.双击FindRoad.bat，即可生成结果myresult.csv在data目录下。相关日志在log目录。
