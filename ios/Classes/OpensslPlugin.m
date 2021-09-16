#import "OpensslPlugin.h"
#if __has_include(<openssl_plugin/openssl_plugin-Swift.h>)
#import <openssl_plugin/openssl_plugin-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "openssl_plugin-Swift.h"
#endif

@implementation OpensslPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftOpensslPlugin registerWithRegistrar:registrar];
}
@end
