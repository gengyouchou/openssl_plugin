import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:openssl_plugin/openssl_plugin.dart';

void main() {
  const MethodChannel channel = MethodChannel('openssl_plugin');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await OpensslPlugin.platformVersion, '42');
  });

}
