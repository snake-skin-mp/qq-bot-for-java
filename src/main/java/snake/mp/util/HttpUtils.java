package snake.mp.util;

import org.apache.commons.io.FileUtils;
import org.apache.http.HttpEntity;
import org.apache.http.HttpHost;
import org.apache.http.client.config.CookieSpecs;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpRequestBase;
import org.apache.http.config.Registry;
import org.apache.http.config.RegistryBuilder;
import org.apache.http.conn.socket.ConnectionSocketFactory;
import org.apache.http.conn.socket.LayeredConnectionSocketFactory;
import org.apache.http.conn.socket.PlainConnectionSocketFactory;
import org.apache.http.conn.ssl.SSLConnectionSocketFactory;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.impl.conn.PoolingHttpClientConnectionManager;
import org.apache.http.util.EntityUtils;

import java.io.File;
import java.io.IOException;
import java.util.Optional;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class HttpUtils {

//    private static final int  threadNum = 16;
//    static ThreadPoolExecutor tpe = new ThreadPoolExecutor(threadNum, threadNum, threadNum, TimeUnit.DAYS, new LinkedBlockingDeque<>());

    private static PoolingHttpClientConnectionManager cm = null;

    static {
        ConnectionSocketFactory plainsf = PlainConnectionSocketFactory.getSocketFactory();
        LayeredConnectionSocketFactory sslsf = SSLConnectionSocketFactory.getSocketFactory();
        Registry<ConnectionSocketFactory> registry = RegistryBuilder
                .<ConnectionSocketFactory>create().register("http", plainsf)
                .register("https", sslsf).build();
        cm = new PoolingHttpClientConnectionManager(registry);
        // 将最大连接数增加
        cm.setMaxTotal(1024);
        // 将每个路由基础的连接增加
//        cm.setDefaultMaxPerRoute(1024);
//        HttpHost httpHost = new HttpHost(hostname, port);
//        // 将目标主机的最大连接数增加
//        cm.setMaxPerRoute(new HttpRoute(httpHost), maxRoute);
    }

    private static RequestConfig defaultRequestConfig = RequestConfig.custom()
            .setSocketTimeout(10000)
            .setConnectTimeout(10000)
            .setConnectionRequestTimeout(10000)
            .setCookieSpec(CookieSpecs.STANDARD)
            .build();
    private static ConcurrentHashMap<String, CloseableHttpClient> httpMap = new ConcurrentHashMap<>();


    /**
     * @param httpRequestBase httpRequestBase
     * @return Response
     */
    private static String doHttpRequest(HttpRequestBase httpRequestBase, String charset) {
        CloseableHttpClient httpClient;
        httpClient = HttpClientBuilder.create().setDefaultRequestConfig(defaultRequestConfig).setConnectionManager(cm).build();
        String ans = "";
        CloseableHttpResponse response = null;
        try {
            response = httpClient.execute(httpRequestBase);
            HttpEntity responseEntity = response.getEntity();
            if (responseEntity != null) {
                ans = EntityUtils.toString(responseEntity, charset);
//System.out.println(httpRequestBase.getURI());
            }
        } catch (IOException e) {
//            if (!proxyIp.equals("")) {
//                httpMap.remove(proxyIp);
//            }
            throw new RuntimeException(e);
//            e.printStackTrace();
        } finally {
            try {
                // 释放资源
                if (response != null) {
                    response.close();
                }
//                if (httpClient != null) {
//                    httpClient.close();
//                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return ans;
    }
}