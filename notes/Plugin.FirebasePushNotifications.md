# .NET 8 MAUI でプッシュ通知をする方法。

Visual Studio で既にアプリのプロジェクト作成し、アプリでの配信の準備をしているものとする。

## 必要な NuGet パッケージを入れる
Visual Studio を起動していない状態で、プロジェクトのディレクトリでコマンドラインを開いて以下を実行し、.NET 6 向けのパッケージを入れる。
```
dotnet add package Xamarin.Firebase.iOS.Core
dotnet add package Xamarin.Firebase.iOS.Installations
```
.csproj に次の設定を加える。

Android で前述のパッケージは不要なので iOS に限定する。
```
<ItemGroup Condition="'$(TargetFramework)'=='net8.0-ios'">
    <PackageReference Include="Xamarin.Firebase.iOS.Core" Version="8.10.0.3" />
    <PackageReference Include="Xamarin.Firebase.iOS.Installations" Version="8.10.0.3" />
</ItemGroup>
```

Visual Studio を起動し、NuGet で以下のパッケージを入れる。
- Plugin.FirebasePushNotifications (thomasgalliker)
- Microsoft.Extensions.Logging.Abstractions

NuGet で Android で必要な以下のパッケージを入れる。
- Xamarin.Firebase.Common
- Xamarin.Firebase.Messaging
- Xamarin.GooglePlayServices.Tasks

MauiProgram.csで.UseFirebasePushNotifications()を呼ぶことで機能が有効になる。
通知の許可、通知の登録方法など他の情報は下のサイトから。

https://github.com/thomasgalliker/Plugin.FirebasePushNotifications

## Firebase の準備
- Firebase Console に https://console.firebase.google.com/ アクセスし、プロジェクトを作成する。

## iOS のための準備
### Apple Developer Console でプッシュ通知の許可と証明書を取得する
- Edit your App ID Configuration で Push Notifications 権限を付けておく。
- Certificates, Identifiers & Profiles の Keys で APNs 証明書を作成する。

### Firebase と連携
- iOS+ というアイコンを押して指示に従ってアプリを追加する。
- GoogleService-Info.plist を指示通りダウンロードして Info.plist と同じフォルダに配置する。
- プロジェクトの設定 → Cloud Messaging タブで APNs 証明書をアップロードする。

.csproj に次の設定で Firebase との連携設定をリソースに加える。
```
<ItemGroup>
    <BundleResource Include="Platforms\iOS\GoogleService-Info.plist" Link="GoogleService-Info.plist" />
</ItemGroup>
```

### プロジェクトの設定
.csproj に次の設定を加える。

リリースビルドで .ipa を生成するようにするため。Publish が失敗する時のため。
```
<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|AnyCPU'">
    <BuildIpa>True</BuildIpa>
    <MtouchLink>SdkOnly</MtouchLink>
    <UseInterpreter>true</UseInterpreter>
</PropertyGroup>
```

Publish が失敗する場合はビルドログの最後で .ipa ファイルの生成場所が表示される。
Mac 上で以下のコマンドを実行して手動アップロードするとよい。
```
xcrun altool --upload-app -f <PROJECT>.ipa -t ios -u <USER> -p <PASSWORD>
```

Info.plist にバックグラウンドで通知を受け取るための設定を加える。
```
<key>UIBackgroundModes</key>
<array>
    <string>remote-notification</string>
</array>
```

Info.plist と同じディレクトリに Entitlements.plist を作成して次の内容を書き込む、
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>aps-environment</key>
    <string>development</string>
</dict>
</plist>
```
なお、リリース時は上記の development を production に変える必要がある。

### AppDelegate の修正
iOS では AppDelegate で IUNUserNotificationCenterDelegate を実装する必要がある。
詳細は IUNUserNotificationCenterDelegate で検索されたし。

## Android のための準備
### Firebase と連携
- Android のアイコンを押して指示に従ってアプリを追加する。
- google-services.json を指示通りダウンロードして Platforms\Android\Resources に配置する。

.csproj に次の設定で Firebase との連携設定をリソースに加える。
```
<ItemGroup>
  <GoogleServicesJson Include="Platforms\Android\Resources\google-services.json" Link="Platforms\Android\Resources\google-services.json" />
</ItemGroup>
```

### FirebaseMessagingService の実装
Android では FirebaseMessagingService を実装する必要がある。
詳細は FirebaseMessagingService で検索されたし。

## プッシュ通知の送信
他の Firebase を利用するアプリと同様、Firebase Console あるいはその API を使って通知を送ることが出来る。

