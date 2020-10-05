export class LocalStorge {
  get(key: string): string;
  set(key: string, data: string): boolean;
  remove(key: string): boolean;
}
