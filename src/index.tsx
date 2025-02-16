import { NitroModules } from 'react-native-nitro-modules';
import type { NitroTor } from './NitroTor.nitro';

const NitroTorHybridObject =
  NitroModules.createHybridObject<NitroTor>('NitroTor');

export function multiply(a: number, b: number): number {
  return NitroTorHybridObject.multiply(a, b);
}
