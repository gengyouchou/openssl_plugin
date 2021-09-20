import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:openssl_plugin/openssl_plugin.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:path_provider/path_provider.dart';
import 'package:ffi/ffi.dart';
import 'dart:ffi'; // For FFI

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

  Future<String> test() async {
    final path = await _localPath;
    Pointer<Int8> nativeValue = path.toNativeUtf8().cast<Int8>();

    nativeAdd(nativeValue, path.length);

    freeFunc(nativeValue);

    return path;
  }

  @override
  Widget build(BuildContext context) {
    requestStoragePermission();
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Text('test openssl:${test()}\n'),
        ),
      ),
    );
  }
}
