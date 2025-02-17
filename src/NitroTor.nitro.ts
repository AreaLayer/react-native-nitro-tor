import type { HybridObject } from 'react-native-nitro-modules';

export interface NitroTor
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  multiply(a: number, b: number): number;
}
