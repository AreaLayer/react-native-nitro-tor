import { NitroModules } from 'react-native-nitro-modules';
import type { Tor as TorSpec } from './Tor.nitro';

export const RnTor = NitroModules.createHybridObject<TorSpec>('Tor');
