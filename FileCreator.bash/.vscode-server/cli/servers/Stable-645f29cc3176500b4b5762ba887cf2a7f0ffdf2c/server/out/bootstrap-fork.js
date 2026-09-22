/*!--------------------------------------------------------
 * Copyright (C) Microsoft Corporation. All rights reserved.
 *--------------------------------------------------------*/
/******************************************************************************
Copyright (c) Microsoft Corporation.

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
***************************************************************************** */
/* global Reflect, Promise, SuppressedError, Symbol, Iterator */

var extendStatics = function(d, b) {
    extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (Object.prototype.hasOwnProperty.call(b, p)) d[p] = b[p]; };
    return extendStatics(d, b);
};

export function __extends(d, b) {
    if (typeof b !== "function" && b !== null)
        throw new TypeError("Class extends value " + String(b) + " is not a constructor or null");
    extendStatics(d, b);
    function __() { this.constructor = d; }
    d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
}

export var __assign = function() {
    __assign = Object.assign || function __assign(t) {
        for (var s, i = 1, n = arguments.length; i < n; i++) {
            s = arguments[i];
            for (var p in s) if (Object.prototype.hasOwnProperty.call(s, p)) t[p] = s[p];
        }
        return t;
    }
    return __assign.apply(this, arguments);
}

export function __rest(s, e) {
    var t = {};
    for (var p in s) if (Object.prototype.hasOwnProperty.call(s, p) && e.indexOf(p) < 0)
        t[p] = s[p];
    if (s != null && typeof Object.getOwnPropertySymbols === "function")
        for (var i = 0, p = Object.getOwnPropertySymbols(s); i < p.length; i++) {
            if (e.indexOf(p[i]) < 0 && Object.prototype.propertyIsEnumerable.call(s, p[i]))
                t[p[i]] = s[p[i]];
        }
    return t;
}

export function __decorate(decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
}

export function __param(paramIndex, decorator) {
    return function (target, key) { decorator(target, key, paramIndex); }
}

export function __esDecorate(ctor, descriptorIn, decorators, contextIn, initializers, extraInitializers) {
    function accept(f) { if (f !== void 0 && typeof f !== "function") throw new TypeError("Function expected"); return f; }
    var kind = contextIn.kind, key = kind === "getter" ? "get" : kind === "setter" ? "set" : "value";
    var target = !descriptorIn && ctor ? contextIn["static"] ? ctor : ctor.prototype : null;
    var descriptor = descriptorIn || (target ? Object.getOwnPropertyDescriptor(target, contextIn.name) : {});
    var _, done = false;
    for (var i = decorators.length - 1; i >= 0; i--) {
        var context = {};
        for (var p in contextIn) context[p] = p === "access" ? {} : contextIn[p];
        for (var p in contextIn.access) context.access[p] = contextIn.access[p];
        context.addInitializer = function (f) { if (done) throw new TypeError("Cannot add initializers after decoration has completed"); extraInitializers.push(accept(f || null)); };
        var result = (0, decorators[i])(kind === "accessor" ? { get: descriptor.get, set: descriptor.set } : descriptor[key], context);
        if (kind === "accessor") {
            if (result === void 0) continue;
            if (result === null || typeof result !== "object") throw new TypeError("Object expected");
            if (_ = accept(result.get)) descriptor.get = _;
            if (_ = accept(result.set)) descriptor.set = _;
            if (_ = accept(result.init)) initializers.unshift(_);
        }
        else if (_ = accept(result)) {
            if (kind === "field") initializers.unshift(_);
            else descriptor[key] = _;
        }
    }
    if (target) Object.defineProperty(target, contextIn.name, descriptor);
    done = true;
};

export function __runInitializers(thisArg, initializers, value) {
    var useValue = arguments.length > 2;
    for (var i = 0; i < initializers.length; i++) {
        value = useValue ? initializers[i].call(thisArg, value) : initializers[i].call(thisArg);
    }
    return useValue ? value : void 0;
};

export function __propKey(x) {
    return typeof x === "symbol" ? x : "".concat(x);
};

export function __setFunctionName(f, name, prefix) {
    if (typeof name === "symbol") name = name.description ? "[".concat(name.description, "]") : "";
    return Object.defineProperty(f, "name", { configurable: true, value: prefix ? "".concat(prefix, " ", name) : name });
};

export function __metadata(metadataKey, metadataValue) {
    if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(metadataKey, metadataValue);
}

export function __awaiter(thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
}

export function __generator(thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g = Object.create((typeof Iterator === "function" ? Iterator : Object).prototype);
    return g.next = verb(0), g["throw"] = verb(1), g["return"] = verb(2), typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (g && (g = 0, op[0] && (_ = 0)), _) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
}

export var __createBinding = Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
        desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
});

export function __exportStar(m, o) {
    for (var p in m) if (p !== "default" && !Object.prototype.hasOwnProperty.call(o, p)) __createBinding(o, m, p);
}

export function __values(o) {
    var s = typeof Symbol === "function" && Symbol.iterator, m = s && o[s], i = 0;
    if (m) return m.call(o);
    if (o && typeof o.length === "number") return {
        next: function () {
            if (o && i >= o.length) o = void 0;
            return { value: o && o[i++], done: !o };
        }
    };
    throw new TypeError(s ? "Object is not iterable." : "Symbol.iterator is not defined.");
}

export function __read(o, n) {
    var m = typeof Symbol === "function" && o[Symbol.iterator];
    if (!m) return o;
    var i = m.call(o), r, ar = [], e;
    try {
        while ((n === void 0 || n-- > 0) && !(r = i.next()).done) ar.push(r.value);
    }
    catch (error) { e = { error: error }; }
    finally {
        try {
            if (r && !r.done && (m = i["return"])) m.call(i);
        }
        finally { if (e) throw e.error; }
    }
    return ar;
}

/** @deprecated */
export function __spread() {
    for (var ar = [], i = 0; i < arguments.length; i++)
        ar = ar.concat(__read(arguments[i]));
    return ar;
}

/** @deprecated */
export function __spreadArrays() {
    for (var s = 0, i = 0, il = arguments.length; i < il; i++) s += arguments[i].length;
    for (var r = Array(s), k = 0, i = 0; i < il; i++)
        for (var a = arguments[i], j = 0, jl = a.length; j < jl; j++, k++)
            r[k] = a[j];
    return r;
}

export function __spreadArray(to, from, pack) {
    if (pack || arguments.length === 2) for (var i = 0, l = from.length, ar; i < l; i++) {
        if (ar || !(i in from)) {
            if (!ar) ar = Array.prototype.slice.call(from, 0, i);
            ar[i] = from[i];
        }
    }
    return to.concat(ar || Array.prototype.slice.call(from));
}

export function __await(v) {
    return this instanceof __await ? (this.v = v, this) : new __await(v);
}

export function __asyncGenerator(thisArg, _arguments, generator) {
    if (!Symbol.asyncIterator) throw new TypeError("Symbol.asyncIterator is not defined.");
    var g = generator.apply(thisArg, _arguments || []), i, q = [];
    return i = Object.create((typeof AsyncIterator === "function" ? AsyncIterator : Object).prototype), verb("next"), verb("throw"), verb("return", awaitReturn), i[Symbol.asyncIterator] = function () { return this; }, i;
    function awaitReturn(f) { return function (v) { return Promise.resolve(v).then(f, reject); }; }
    function verb(n, f) { if (g[n]) { i[n] = function (v) { return new Promise(function (a, b) { q.push([n, v, a, b]) > 1 || resume(n, v); }); }; if (f) i[n] = f(i[n]); } }
    function resume(n, v) { try { step(g[n](v)); } catch (e) { settle(q[0][3], e); } }
    function step(r) { r.value instanceof __await ? Promise.resolve(r.value.v).then(fulfill, reject) : settle(q[0][2], r); }
    function fulfill(value) { resume("next", value); }
    function reject(value) { resume("throw", value); }
    function settle(f, v) { if (f(v), q.shift(), q.length) resume(q[0][0], q[0][1]); }
}

export function __asyncDelegator(o) {
    var i, p;
    return i = {}, verb("next"), verb("throw", function (e) { throw e; }), verb("return"), i[Symbol.iterator] = function () { return this; }, i;
    function verb(n, f) { i[n] = o[n] ? function (v) { return (p = !p) ? { value: __await(o[n](v)), done: false } : f ? f(v) : v; } : f; }
}

export function __asyncValues(o) {
    if (!Symbol.asyncIterator) throw new TypeError("Symbol.asyncIterator is not defined.");
    var m = o[Symbol.asyncIterator], i;
    return m ? m.call(o) : (o = typeof __values === "function" ? __values(o) : o[Symbol.iterator](), i = {}, verb("next"), verb("throw"), verb("return"), i[Symbol.asyncIterator] = function () { return this; }, i);
    function verb(n) { i[n] = o[n] && function (v) { return new Promise(function (resolve, reject) { v = o[n](v), settle(resolve, reject, v.done, v.value); }); }; }
    function settle(resolve, reject, d, v) { Promise.resolve(v).then(function(v) { resolve({ value: v, done: d }); }, reject); }
}

export function __makeTemplateObject(cooked, raw) {
    if (Object.defineProperty) { Object.defineProperty(cooked, "raw", { value: raw }); } else { cooked.raw = raw; }
    return cooked;
};

var __setModuleDefault = Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
};

var ownKeys = function(o) {
    ownKeys = Object.getOwnPropertyNames || function (o) {
        var ar = [];
        for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
        return ar;
    };
    return ownKeys(o);
};

export function __importStar(mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
    __setModuleDefault(result, mod);
    return result;
}

export function __importDefault(mod) {
    return (mod && mod.__esModule) ? mod : { default: mod };
}

export function __classPrivateFieldGet(receiver, state, kind, f) {
    if (kind === "a" && !f) throw new TypeError("Private accessor was defined without a getter");
    if (typeof state === "function" ? receiver !== state || !f : !state.has(receiver)) throw new TypeError("Cannot read private member from an object whose class did not declare it");
    return kind === "m" ? f : kind === "a" ? f.call(receiver) : f ? f.value : state.get(receiver);
}

export function __classPrivateFieldSet(receiver, state, value, kind, f) {
    if (kind === "m") throw new TypeError("Private method is not writable");
    if (kind === "a" && !f) throw new TypeError("Private accessor was defined without a setter");
    if (typeof state === "function" ? receiver !== state || !f : !state.has(receiver)) throw new TypeError("Cannot write private member to an object whose class did not declare it");
    return (kind === "a" ? f.call(receiver, value) : f ? f.value = value : state.set(receiver, value)), value;
}

export function __classPrivateFieldIn(state, receiver) {
    if (receiver === null || (typeof receiver !== "object" && typeof receiver !== "function")) throw new TypeError("Cannot use 'in' operator on non-object");
    return typeof state === "function" ? receiver === state : state.has(receiver);
}

export function __addDisposableResource(env, value, async) {
    if (value !== null && value !== void 0) {
        if (typeof value !== "object" && typeof value !== "function") throw new TypeError("Object expected.");
        var dispose, inner;
        if (async) {
            if (!Symbol.asyncDispose) throw new TypeError("Symbol.asyncDispose is not defined.");
            dispose = value[Symbol.asyncDispose];
        }
        if (dispose === void 0) {
            if (!Symbol.dispose) throw new TypeError("Symbol.dispose is not defined.");
            dispose = value[Symbol.dispose];
            if (async) inner = dispose;
        }
        if (typeof dispose !== "function") throw new TypeError("Object not disposable.");
        if (inner) dispose = function() { try { inner.call(this); } catch (e) { return Promise.reject(e); } };
        env.stack.push({ value: value, dispose: dispose, async: async });
    }
    else if (async) {
        env.stack.push({ async: true });
    }
    return value;

}

var _SuppressedError = typeof SuppressedError === "function" ? SuppressedError : function (error, suppressed, message) {
    var e = new Error(message);
    return e.name = "SuppressedError", e.error = error, e.suppressed = suppressed, e;
};

export function __disposeResources(env) {
    function fail(e) {
        env.error = env.hasError ? new _SuppressedError(e, env.error, "An error was suppressed during disposal.") : e;
        env.hasError = true;
    }
    var r, s = 0;
    function next() {
        while (r = env.stack.pop()) {
            try {
                if (!r.async && s === 1) return s = 0, env.stack.push(r), Promise.resolve().then(next);
                if (r.dispose) {
                    var result = r.dispose.call(r.value);
                    if (r.async) return s |= 2, Promise.resolve(result).then(next, function(e) { fail(e); return next(); });
                }
                else s |= 1;
            }
            catch (e) {
                fail(e);
            }
        }
        if (s === 1) return env.hasError ? Promise.reject(env.error) : Promise.resolve();
        if (env.hasError) throw env.error;
    }
    return next();
}

export function __rewriteRelativeImportExtension(path, preserveJsx) {
    if (typeof path === "string" && /^\.\.?\//.test(path)) {
        return path.replace(/\.(tsx)$|((?:\.d)?)((?:\.[^./]+?)?)\.([cm]?)ts$/i, function (m, tsx, d, ext, cm) {
            return tsx ? preserveJsx ? ".jsx" : ".js" : d && (!ext || !cm) ? m : (d + ext + "." + cm.toLowerCase() + "js");
        });
    }
    return path;
}

export default {
    __extends: __extends,
    __assign: __assign,
    __rest: __rest,
    __decorate: __decorate,
    __param: __param,
    __esDecorate: __esDecorate,
    __runInitializers: __runInitializers,
    __propKey: __propKey,
    __setFunctionName: __setFunctionName,
    __metadata: __metadata,
    __awaiter: __awaiter,
    __generator: __generator,
    __createBinding: __createBinding,
    __exportStar: __exportStar,
    __values: __values,
    __read: __read,
    __spread: __spread,
    __spreadArrays: __spreadArrays,
    __spreadArray: __spreadArray,
    __await: __await,
    __asyncGenerator: __asyncGenerator,
    __asyncDelegator: __asyncDelegator,
    __asyncValues: __asyncValues,
    __makeTemplateObject: __makeTemplateObject,
    __importStar: __importStar,
    __importDefault: __importDefault,
    __classPrivateFieldGet: __classPrivateFieldGet,
    __classPrivateFieldSet: __classPrivateFieldSet,
    __classPrivateFieldIn: __classPrivateFieldIn,
    __addDisposableResource: __addDisposableResource,
    __disposeResources: __disposeResources,
    __rewriteRelativeImportExtension: __rewriteRelativeImportExtension,
};

function v(e){let t=[];typeof e=="number"&&t.push("code/timeOrigin",e);function f(s,n){t.push(s,n?.startTime??Date.now())}function m(){let s=[];for(let n=0;n<t.length;n+=2)s.push({name:t[n],startTime:t[n+1]});return s}function l(s){if(typeof s>"u"){let n=t.length>=2&&t[0]==="code/timeOrigin",i=n?t[1]:void 0;t.length=0,n&&t.push("code/timeOrigin",i)}else for(let n=t.length-2;n>=0;n-=2)t[n]===s&&t.splice(n,2)}return{mark:f,getMarks:m,clearMarks:l}}function J(){if(typeof performance=="object"&&typeof performance.mark=="function"&&!performance.nodeTiming)return typeof performance.timeOrigin!="number"&&!performance.timing?v():{mark(e,t){performance.mark(e,t)},clearMarks(e){performance.clearMarks(e)},getMarks(){let e=performance.timeOrigin;typeof e!="number"&&(e=(performance.timing.navigationStart||performance.timing.redirectStart||performance.timing.fetchStart)??0);let t=[{name:"code/timeOrigin",startTime:Math.round(e)}];for(let f of performance.getEntriesByType("mark"))t.push({name:f.name,startTime:Math.round(e+f.startTime)});return t}};if(typeof process=="object"){let e=performance?.timeOrigin;return v(e)}else return console.trace("perf-util loaded in UNKNOWN environment"),v()}function x(e){return e.MonacoPerformanceMarks||(e.MonacoPerformanceMarks=J()),e.MonacoPerformanceMarks}var y=x(globalThis),O=y.mark,ce=y.clearMarks,le=y.getMarks;import*as h from"node:path";import{Buffer as W}from"node:buffer";import{createRequire as q}from"node:module";var D=q(import.meta.url),b=process.platform==="win32";process.platform==="linux"&&(W.poolSize=8*1024);Error.stackTraceLimit=100;if(!process.env.VSCODE_HANDLES_SIGPIPE){let e=!1;process.on("SIGPIPE",()=>{e||(e=!0,console.error(new Error("Unexpected SIGPIPE")))})}function K(){try{typeof process.env.VSCODE_CWD!="string"&&(process.env.VSCODE_CWD=process.cwd()),process.platform==="win32"&&process.chdir(h.dirname(process.execPath))}catch(e){console.error(e)}}K();function X(){if(!process.env.ELECTRON_RUN_AS_NODE&&!process.versions.electron||process.env.VSCODE_DEV)return;let e=l=>{if(b&&l.length>=2&&l.charCodeAt(1)===58){let s=l.charCodeAt(0);if(s>=65&&s<=90||s>=97&&s<=122)return l[0].toLowerCase()+l.slice(1)}return l},t=e(h.join(import.meta.dirname,"../node_modules")),f=D("node:module"),m=f._resolveLookupPaths;f._resolveLookupPaths=function(l,s){let n=m(l,s);if(Array.isArray(n)){for(let i=0,a=n.length;i<a;i++)if(e(n[i])===t){n.splice(i,0,`${n[i]}.asar`);break}}return n}}X();function w(e){if(!process.env.VSCODE_DEV)return;if(!e)throw new Error("Missing injectPath");D("node:module").register("./bootstrap-import.js",{parentURL:import.meta.url,data:e})}function U(){if(typeof process?.versions?.electron=="string")return;let e=D("module"),t=e.globalPaths,f=e._resolveLookupPaths;e._resolveLookupPaths=function(l,s){let n=f(l,s);if(Array.isArray(n)){let i=0;for(;i<n.length&&n[n.length-1-i]===t[t.length-1-i];)i++;return n.slice(0,n.length-i)}return n};let m=e._nodeModulePaths;e._nodeModulePaths=function(l){let s=m(l);if(!b)return s;let n=i=>i.length>=3&&i.endsWith(":\\");if(n(l)||(s=s.filter(i=>!n(h.dirname(i)))),process.env.HOMEDRIVE&&process.env.HOMEPATH){let i=h.dirname(h.join(process.env.HOMEDRIVE,process.env.HOMEPATH)),a=d=>h.relative(d,i).length===0;a(l)||(s=s.filter(d=>!a(h.dirname(d))))}return s}}import*as S from"node:fs";import{createRequire as j,isBuiltin as Y,registerHooks as Q}from"node:module";import{dirname as Z,join as ee}from"node:path";import{fileURLToPath as k,pathToFileURL as F}from"node:url";import{createRequire as z}from"node:module";var R=z(import.meta.url),_={BUILD_INSERT_PRODUCT_CONFIGURATION:"BUILD_INSERT_PRODUCT_CONFIGURATION"};_.BUILD_INSERT_PRODUCT_CONFIGURATION&&(_=R("../product.json"));var L={"name":"Code","version":"1.137.0","private":true,"overrides":{"node-gyp-build":"4.8.1","kerberos@2.1.1":{"node-addon-api":"7.1.0"},"yauzl":"^3.3.1"},"allowScripts":{"kerberos@2.1.1":true,"koffi@3.1.2":false,"node-pty@1.2.0-beta.15":true,"@parcel/watcher@2.5.6":true,"@vscode/deviceid@0.1.5":true,"@vscode/fs-copyfile@2.0.0":true,"@vscode/native-watchdog@1.4.6":true,"@vscode/spdlog@0.15.8":true,"@vscode/windows-registry@1.2.0":true,"ssh2":false,"@vscode/sqlite3@5.1.12-vscode":true,"cpu-features":false,"@vscode/windows-ca-certs@0.3.4":true,"@vscode/windows-process-tree@0.8.0":true},"type":"module"};L.BUILD_INSERT_PACKAGE_CONFIGURATION&&(L=R("../package.json"));var V={};if(process.env.VSCODE_DEV)try{V=R("../product.overrides.json"),_=Object.assign(_,V)}catch{}var G=_,$=L;globalThis._VSCODE_PRODUCT_JSON={...G};globalThis._VSCODE_PACKAGE_JSON={...$};globalThis._VSCODE_FILE_ROOT=import.meta.dirname;function re(){if(!process.env.ELECTRON_RUN_AS_NODE&&!process.versions.electron)return;let e,t=process.env.VSCODE_ASAR_TRACE||void 0;if(t){let r="[asar-resolve] ";t==="1"||t==="true"||t==="on"||t==="stderr"?e=o=>{try{process.stderr.write(`${r}${o}
`)}catch{}}:e=o=>{try{S.appendFileSync(t,`${r}${o}
`)}catch{}}}let f=r=>process.platform==="win32"&&r.length>=2&&(r.charCodeAt(0)>=65&&r.charCodeAt(0)<=90||r.charCodeAt(0)>=97&&r.charCodeAt(0)<=122)&&r.charCodeAt(1)===58?r[0].toLowerCase()+r.slice(1):r,m=r=>{if(r==="")return!1;let o=r[0];return o==="."||o==="/"||o==="#"?!1:!URL.canParse(r)},l=r=>{if(r[0]==="@"){let c=r.indexOf("/");if(c===-1)return r;let u=r.indexOf("/",c+1);return u===-1?r:r.slice(0,u)}let o=r.indexOf("/");return o===-1?r:r.slice(0,o)},s=Z(import.meta.dirname),n=process.env.VSCODE_DEV?void 0:f(s),i=n?j(ee(s,"node_modules.asar","x.js")):void 0;e?.(`tracing enabled (node ${process.versions.node}); resourcesPath=${n}`);let a,d=!0;Q({resolve(r,o,c){if(d)return d=!1,a=[...o.conditions],c(r,o);let u=a;if(o.importAttributes===void 0||u!==void 0&&o.conditions.length===u.length&&o.conditions.every((E,g)=>E===u[g]))return c(r,o);if(r==="fs")return e?.(`map "fs" -> node:original-fs (from ${o.parentURL})`),{format:"builtin",shortCircuit:!0,url:"node:original-fs"};if(i&&n&&o.parentURL&&m(r)&&!Y(r)&&r!=="electron"&&!r.startsWith("electron/")){let E;try{E=f(k(o.parentURL))}catch{E=void 0}if(E&&E.startsWith(n)){e?.(`resolve "${r}" from "${o.parentURL}"`);let g,N;try{g=c(r,o)}catch(p){N=p instanceof Error?p:new Error(String(p))}if(g){let p;try{p=f(k(g.url))}catch{p=void 0}if(!p||p.startsWith(n))return e?.(`  default -> ${g.url} (in app, ACCEPT)`),g;e?.(`  default -> ${g.url} (outside app, reject)`)}else e?.(`  default -> <none> (${N?.message})`);let T;try{T=i.resolve(`./${l(r)}/package.json`)}catch{throw e?.(`  archive: package "${l(r)}" NOT in archive -> throw`),N??new Error(`Cannot find package '${r}' within the application resources`)}e?.(`  archive pkg.json -> ${T}`);try{let p=c(r,{...o,parentURL:F(T).href}),P;try{P=f(k(p.url))}catch{P=void 0}if(P&&P.startsWith(n))return e?.(`  self-ref -> ${p.url} (in app, ACCEPT)`),p;e?.(`  self-ref -> ${p.url} (escaped app, reject)`)}catch(p){e?.(`  self-ref -> <throw> (${p instanceof Error?p.message:String(p)})`)}let H=i.resolve(`./${r}`),I=F(H).href;return e?.(`  direct -> ${I} (ACCEPT)`),{url:I,shortCircuit:!0}}e?.(`defer "${r}" (parent outside app resources: ${o.parentURL})`)}return c(r,o)}});try{j(import.meta.url).resolve("node:fs")}finally{d=!1}if(!a)throw new Error("Failed to identify CommonJS module resolution conditions")}re();var A;function ne(){return A||(A=te()),A}async function te(){O("code/willLoadNls");let e,t;if(process.env.VSCODE_NLS_CONFIG)try{e=JSON.parse(process.env.VSCODE_NLS_CONFIG),e?.languagePack?.messagesFile?t=e.languagePack.messagesFile:e?.defaultMessagesFile&&(t=e.defaultMessagesFile),globalThis._VSCODE_NLS_LANGUAGE=e?.resolvedLanguage}catch(f){console.error(`Error reading VSCODE_NLS_CONFIG from environment: ${f}`)}if(!(process.env.VSCODE_DEV||!t)){try{globalThis._VSCODE_NLS_MESSAGES=JSON.parse((await S.promises.readFile(t)).toString())}catch(f){if(console.error(`Error reading NLS messages file ${t}: ${f}`),e?.languagePack?.corruptMarkerFile)try{await S.promises.writeFile(e.languagePack.corruptMarkerFile,"corrupted")}catch(m){console.error(`Error writing corrupted NLS marker file: ${m}`)}if(e?.defaultMessagesFile&&e.defaultMessagesFile!==t)try{globalThis._VSCODE_NLS_MESSAGES=JSON.parse((await S.promises.readFile(e.defaultMessagesFile)).toString())}catch(m){console.error(`Error reading default NLS messages file ${e.defaultMessagesFile}: ${m}`)}}return O("code/didLoadNls"),e}}async function B(){await ne()}O("code/fork/start");function oe(){function f(a){let d=[],r=[];if(a.length)for(let o=0;o<a.length;o++){let c=a[o];if(typeof c>"u")c="undefined";else if(c instanceof Error){let u=c;u.stack?c=u.stack:c=u.toString()}r.push(c)}try{let o=JSON.stringify(r,function(c,u){if(l(u)||Array.isArray(u)){if(d.indexOf(u)!==-1)return"[Circular]";d.push(u)}return u});return o.length>1e5?"Output omitted for a large object that exceeds the limits":o}catch(o){return`Output omitted for an object that cannot be inspected ('${o.toString()}')`}}function m(a){try{process.send&&process.send(a)}catch{}}function l(a){return typeof a=="object"&&a!==null&&!Array.isArray(a)&&!(a instanceof RegExp)&&!(a instanceof Date)}function s(a,d){m({type:"__$console",severity:a,arguments:d})}function n(a,d){Object.defineProperty(console,a,{set:()=>{},get:()=>function(){s(d,f(arguments))}})}function i(a,d){let r=process[a],o=r.write,c="";Object.defineProperty(r,"write",{set:()=>{},get:()=>(u,C,E)=>{c+=u.toString(C);let g=c.length>1048576?c.length:c.lastIndexOf(`
`);g!==-1&&(console[d](c.slice(0,g)),c=c.slice(g+1)),o.call(r,u,C,E)}})}process.env.VSCODE_VERBOSE_LOGGING==="true"?(n("info","log"),n("log","log"),n("warn","warn"),n("error","error")):(console.log=function(){},console.warn=function(){},console.info=function(){},n("error","error")),i("stderr","error"),i("stdout","log")}function se(){process.on("uncaughtException",function(e){console.error("Uncaught Exception: ",e)}),process.on("unhandledRejection",function(e){console.error("Unhandled Promise Rejection: ",e)})}function ie(){let e=Number(process.env.VSCODE_PARENT_PID);typeof e=="number"&&!isNaN(e)&&setInterval(function(){try{process.kill(e,0)}catch{process.exit()}},5e3)}function ae(){let e=process.env.VSCODE_CRASH_REPORTER_PROCESS_TYPE;if(e)try{process.crashReporter&&typeof process.crashReporter.addExtraParameter=="function"&&process.crashReporter.addExtraParameter("processType",e)}catch(t){console.error(t)}}ae();U();process.env.VSCODE_DEV_INJECT_NODE_MODULE_LOOKUP_PATH&&w(process.env.VSCODE_DEV_INJECT_NODE_MODULE_LOOKUP_PATH);process.send&&process.env.VSCODE_PIPE_LOGGING==="true"&&oe();process.env.VSCODE_HANDLES_UNCAUGHT_ERRORS||se();process.env.VSCODE_PARENT_PID&&ie();await B();await import([`./${process.env.VSCODE_ESM_ENTRYPOINT}.js`].join("/"));
//# sourceMappingURL=https://main.vscode-cdn.net/sourcemaps/645f29cc3176500b4b5762ba887cf2a7f0ffdf2c/core/bootstrap-fork.js.map
