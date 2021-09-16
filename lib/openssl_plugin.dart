
import 'dart:async';

import 'package:flutter/services.dart';

import 'dart:ffi'; // For FFI
import 'dart:io'; // For Platform.isX


final DynamicLibrary nativeAddLib = Platform.isAndroid
    ? DynamicLibrary.open("libnative-lib.so")
    : DynamicLibrary.process();

class OpensslPlugin {
  static const MethodChannel _channel = MethodChannel('openssl_plugin');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
