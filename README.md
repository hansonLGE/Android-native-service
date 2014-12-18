Android-native-service
======================

 It is a native system server which is summed up by the media server and the camera service. Multi clients can asynchronously  connect to a native service by Bp and Bn. It includes the client test code and server code. The source code  also include binder linkToDeath and callback, which is a thoughtfult framework.

 All the people can directly use the native framework as a new server in android platform, and add other service to the server.

 please copy the project to the folder /frameworks in the android source code, and compile. client file [myclient] and server file [myserver] would be generated. 
 
 Please copy myclient & my server to out\target\product\***\root\system\bin\ and test the server in terminal, run the myserver before the myclient. In release version, please add the code [service myserver /system/bin/myserver...] in init.{hardware}.rc.
