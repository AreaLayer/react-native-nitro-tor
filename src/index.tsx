import { NitroModules } from 'react-native-nitro-modules';
import type { Math as MathSpec } from './Math.nitro';
import type { Tor as TorSpec } from './Tor.nitro';

export const RnMath = NitroModules.createHybridObject<MathSpec>('Math');
export const RnTor = NitroModules.createHybridObject<TorSpec>('Tor');
