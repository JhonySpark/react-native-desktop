
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QScopedPointer>
#include <QUrl>

class QQuickItem;
class QQmlEngine;
class QNetworkAccessManager;
class ModuleData;
class UIManager;
class ImageLoader;
class EventDispatcher;
class RedboxItem;
class TestModule;

class BridgePrivate;
class Bridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(bool jsAppStarted READ jsAppStarted NOTIFY jsAppStartedChanged)
    Q_PROPERTY(QQuickItem* visualParent READ visualParent WRITE setVisualParent)
    Q_PROPERTY(QQmlEngine* qmlEngine READ qmlEngine WRITE setQmlEngine)
    Q_PROPERTY(QNetworkAccessManager* networkAccessManager READ networkAccessManager WRITE setNetworkAccessManager)
    Q_PROPERTY(QUrl bundleUrl READ bundleUrl WRITE setBundleUrl)
    Q_PROPERTY(QString pluginsPath READ pluginsPath WRITE setPluginsPath)
    Q_PROPERTY(QString executorName READ executorName WRITE setExecutorName)
    Q_PROPERTY(QList<ModuleData*> modules READ modules)
    Q_PROPERTY(UIManager* uiManager READ uiManager)
    Q_PROPERTY(ImageLoader* imageLoader READ imageLoader)

    enum Fields { FieldRequestModuleIDs, FieldMethodIDs, FieldParams };

    Q_DECLARE_PRIVATE(Bridge)

public:
    Bridge(QObject* parent = 0);
    ~Bridge();

    void init();
    void reload();
    void loadBundle(const QUrl& bundleUrl);

    void invokePromiseCallback(double callbackCode, const QVariantList& args);
    void enqueueJSCall(const QString& module, const QString& method, const QVariantList& args);
    void invokeAndProcess(const QString& method, const QVariantList& args);
    void executeSourceCode(const QByteArray& sourceCode);
    void enqueueRunAppCall(const QVariantList& args);

    // XXX: maybe rename
    bool ready() const;
    void setReady(bool ready);

    bool jsAppStarted() const;

    QQuickItem* visualParent() const;
    void setVisualParent(QQuickItem* item);

    QQmlEngine* qmlEngine() const;
    void setQmlEngine(QQmlEngine* qmlEngine);

    QNetworkAccessManager* networkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager* nam);

    QUrl bundleUrl() const;
    void setBundleUrl(const QUrl& bundleUrl);

    QString pluginsPath() const;
    void setPluginsPath(const QString& pluginsPath);

    QString executorName() const;
    void setExecutorName(const QString& executorName);

    EventDispatcher* eventDispatcher() const;
    QList<ModuleData*> modules() const;
    UIManager* uiManager() const;
    TestModule* testModule() const;
    ImageLoader* imageLoader() const;
    RedboxItem* redbox();

Q_SIGNALS:
    void readyChanged();
    void jsAppStartedChanged();

private Q_SLOTS:
    void sourcesFinished();
    void sourcesLoadFailed();
    void applicationScriptDone();

private:
    void loadSource();
    void initModules();
    void injectModules();
    void processResult(const QJsonDocument& document);
    void setupExecutor();
    void setJsAppStarted(bool started);
    void invokeModuleMethod(int moduleId, int methodId, QList<QVariant> args);

    QScopedPointer<BridgePrivate> d_ptr;
};

#endif // BRIDGE_H