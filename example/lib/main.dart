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
  runApp(const MyApp());
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

  // Ask permission
  requestStoragePermission() async {
    //await super.initState();
    //bool rootAccess = await RootAccess.requestRootAccess;
    //print('root is:$rootAccess');
    bool _isGranted = true;
    if (!await Permission.storage.isGranted) {
      PermissionStatus result = await Permission.storage.request();
      if (result.isGranted) {
        setState(() {
          _isGranted = true;
        });
      } else {
        _isGranted = false;
      }
    }
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
  String Keyname = "";

  void path_file_picket() async {
    String path = "";
    await path_picker().then((value) {
      path = value;
    });
    print("fuck1" + path);
    String name = "";
    await file_picker().then((value) {
      name = value;
    });
    keypath = path + "/" + name;

    print("fuck2" + keypath);
  }

  Future<String> generateRSAKeypair() async {
    //final path = await _localPath;

    final path = await path_picker();
    Pointer<Int8> nativeValue = path.toNativeUtf8().cast<Int8>();

    nativeAdd(nativeValue);

    freeFunc(nativeValue);

    return path;
  }

  Future<String> Encrypt() async {
    final key_path = await _localPath + "/pu.txt";

    //final key_path = await file_picker();
    Pointer<Int8> key_nativeValue = key_path.toNativeUtf8().cast<Int8>();

    final crypt_path = await file_picker();
    Pointer<Int8> crypt_nativeValue = crypt_path.toNativeUtf8().cast<Int8>();

    encrypt(key_nativeValue, crypt_nativeValue);

    freeFunc(key_nativeValue);
    freeFunc(crypt_nativeValue);

    return crypt_path;
  }

  Future<String> Decrypt() async {
    final key_path = await _localPath + "/pr.txt";

    //final key_path = await file_picker();
    Pointer<Int8> key_nativeValue = key_path.toNativeUtf8().cast<Int8>();

    final crypt_path = await file_picker();
    Pointer<Int8> crypt_nativeValue = crypt_path.toNativeUtf8().cast<Int8>();

    decrypt(key_nativeValue, crypt_nativeValue);

    freeFunc(key_nativeValue);
    freeFunc(crypt_nativeValue);

    return crypt_path;
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Text("test"),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              (true
                  ? FlatButton(
                      child: Text('create RSA key pair'),
                      onPressed: generateRSAKeypair,
                      color: Colors.green,
                    )
                  : Container()),
              ((keypath == "") == true
                  ? RaisedButton(
                      child: Text('keypath:' + keypath),
                      onPressed: path_file_picket,
                      color: Colors.red,
                    )
                  : RaisedButton(
                      child: Text('keypath:' + keypath),
                      onPressed: path_file_picket,
                      color: Colors.green,
                    )),
              (true
                  ? FlatButton(
                      child: Text('Encrypt'),
                      onPressed: path_picker,
                      color: Colors.green,
                    )
                  : Container()),
              (true
                  ? FlatButton(
                      child: Text('Decrypt'),
                      onPressed: file_picker,
                      color: Colors.blue,
                    )
                  : FlatButton(
                      child: Text('Start encrypting'),
                      onPressed: file_picker,
                      color: Colors.green,
                    )),
            ],
          ),
        ),
      ),
    );
  }
}
