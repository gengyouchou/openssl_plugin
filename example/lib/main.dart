import 'dart:io';

import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:openssl_plugin/openssl_plugin.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:path_provider/path_provider.dart';
import 'package:ffi/ffi.dart';
import 'dart:ffi'; // For FFI
import 'package:file_picker/file_picker.dart';

void main() async {
  requestStoragePermission();
  runApp(const MyApp());
}

// Ask permission
requestStoragePermission() async {
  if (!await Permission.storage.isGranted) {
    PermissionStatus result = await Permission.storage.request();
    if (result.isGranted) {
      print("isGranted");
    }
  }
}
// Future<void> startup() async {
// 	List<PermissionName> permissionNames = [];
//     permissionNames.add(PermissionName.Location);
//     permissionNames.add(PermissionName.Camera);
//     permissionNames.add(PermissionName.Storage);
//     List<Permissions> permissions = await Permission.requestPermissions(permissionNames);
//     runApp(MyApp());
// }

class UpdateText extends StatefulWidget {
  UpdateTextState createState() => UpdateTextState();
}

class UpdateTextState extends State {
  String textHolder = 'Old Sample Text...!!!';

  changeText() {
    setState(() {
      textHolder = 'New Sample Text...';
    });
  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    throw UnimplementedError();
  }
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';

  @override
  void initState() {
    super.initState();
    initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    // Platform messages may fail, so we use a try/catch PlatformException.
    // We also handle the message potentially returning null.
    try {
      platformVersion =
          await OpensslPlugin.platformVersion ?? 'Unknown platform version';
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
    });
  }

  Future<String> get _localPath async {
    final directory = await getExternalStorageDirectory();

    return directory!.path;
  }

  Future<String> path_picker() async {
    String path = "";
    String? selectedDirectory = await FilePicker.platform
        .getDirectoryPath()
        .then((value) => path = value!);

    if (selectedDirectory != null) {
      // User canceled the picker
    } else {
      print("-1");
    }

    print(path);

    return path;
  }

  Future<String> file_picker() async {
    FilePickerResult? result = await FilePicker.platform.pickFiles();

    String path = "";

    if (result != null) {
      PlatformFile file = result.files.first;

      print(file.name);
      print(file.bytes);
      print(file.size);
      print(file.extension);
      print(file.path);
      path = file.name;
    } else {
      // User canceled the picker
    }

    return path;
  }

  String keypath = "";
  String newkeypath = "";
  String encryptpath = "";
  String decryptpath = "";

  void path_file_picket() async {
    String path = "";
    await path_picker().then((value) {
      path = value;
    });
    print("fuck1");
    String name = "";
    await file_picker().then((value) {
      name = value;
    });
    print(path + "/" + name);

    print("fuck2");
    setState(() {
      keypath = path + "/" + name;
    });
  }

  Future<String> generateRSAKeypair() async {
    //final path = await _localPath;

    final path = await path_picker();
    Pointer<Int8> nativeValue = path.toNativeUtf8().cast<Int8>();

    nativeAdd(nativeValue);

    freeFunc(nativeValue);

    setState(() {
      newkeypath = path;
    });

    return path;
  }

  Future<String> Encrypt() async {
    final key_path = keypath; //await _localPath + "/pu.txt";

    String path = "";
    await path_picker().then((value) {
      path = value;
    });

    String name = "";
    await file_picker().then((value) {
      name = value;
    });
    print(path + "/" + name);

    setState(() {
      encryptpath = path + "/" + name;
    });

    //final key_path = await file_picker();
    Pointer<Int8> key_nativeValue = key_path.toNativeUtf8().cast<Int8>();

    //final crypt_path = await file_picker();
    Pointer<Int8> crypt_nativeValue = encryptpath.toNativeUtf8().cast<Int8>();

    encrypt(key_nativeValue, crypt_nativeValue);

    freeFunc(key_nativeValue);
    freeFunc(crypt_nativeValue);

    return encryptpath;
  }

  Future<String> Decrypt() async {
    final key_path = keypath; //await _localPath + "/pu.txt";

    String path = "";
    await path_picker().then((value) {
      path = value;
    });

    String name = "";
    await file_picker().then((value) {
      name = value;
    });
    print(path + "/" + name);

    setState(() {
      decryptpath = path + "/" + name;
    });

    //final key_path = await file_picker();
    Pointer<Int8> key_nativeValue = key_path.toNativeUtf8().cast<Int8>();

    //final crypt_path = await file_picker();
    Pointer<Int8> crypt_nativeValue = decryptpath.toNativeUtf8().cast<Int8>();

    decrypt(key_nativeValue, crypt_nativeValue);

    freeFunc(key_nativeValue);
    freeFunc(crypt_nativeValue);

    return decryptpath;
  }

  @override
  Widget build(BuildContext context) {
    requestStoragePermission;
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Text("test"),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              FlatButton(
                child: Text('create RSA key pair'),
                onPressed: generateRSAKeypair,
                color: Colors.green,
              ),
              Expanded(
                child: Container(
                  child: Text(newkeypath),
                  color: Colors.amber,
                  height: 20,
                ),
              ),
              RaisedButton(
                child: Text('keypath:'),
                onPressed: path_file_picket,
                color: Colors.red,
              ),
              Expanded(
                child: Container(
                  child: Text(keypath),
                  color: Colors.amber,
                  height: 20,
                ),
              ),
              FlatButton(
                child: Text('Encrypt'),
                onPressed: Encrypt,
                color: Colors.green,
              ),
              Expanded(
                child: Container(
                  child: Text(encryptpath),
                  color: Colors.amber,
                  height: 20,
                ),
              ),
              FlatButton(
                child: Text('Decrypt'),
                onPressed: Decrypt,
                color: Colors.blue,
              ),
              Expanded(
                child: Container(
                  child: Text(decryptpath),
                  color: Colors.amber,
                  height: 20,
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
