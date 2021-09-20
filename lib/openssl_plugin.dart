import 'dart:async';
import 'package:flutter/services.dart';
import 'dart:ffi'; // For FFI
import 'dart:io'; // For Platform.isX

final DynamicLibrary nativeAddLib = Platform.isAndroid
    ? DynamicLibrary.open("libnative-lib.so")
    : DynamicLibrary.process();

typedef Native_reverseString = Int32 Function(Pointer<Int8>, Int32);
typedef FFI_reverseString = int Function(Pointer<Int8>, int);

typedef Native_freeString = Void Function(Pointer<Int8>);
typedef FFI_freeString = void Function(Pointer<Int8>);

FFI_reverseString nativeAdd =
    nativeAddLib.lookupFunction<Native_reverseString, FFI_reverseString>(
        'createRsakeypair');

FFI_freeString freeFunc = nativeAddLib
    .lookupFunction<Native_freeString, FFI_freeString>("free_string");

class OpensslPlugin {
  static const MethodChannel _channel = MethodChannel('openssl_plugin');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
