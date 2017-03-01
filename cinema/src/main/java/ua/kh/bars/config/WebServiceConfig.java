package ua.kh.bars.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.ClassPathResource;
import org.springframework.ws.config.annotation.EnableWs;
import org.springframework.ws.config.annotation.WsConfigurerAdapter;
import org.springframework.ws.wsdl.wsdl11.DefaultWsdl11Definition;
import org.springframework.xml.xsd.SimpleXsdSchema;
import org.springframework.xml.xsd.XsdSchema;

/**
 * Created by green on 4/10/16.
 */

@EnableWs
@Configuration
public class WebServiceConfig extends WsConfigurerAdapter {

    // TODO: move into properties file
    private static final String LOCATION_URI = "http://localhost:8080/ws/";

    @Bean(name = "users")
    public DefaultWsdl11Definition defaultWsdl11DefinitionUsers(XsdSchema usersSchema){
        DefaultWsdl11Definition wsdl11Definition = new DefaultWsdl11Definition();
        wsdl11Definition.setPortTypeName("usersPort");
        wsdl11Definition.setLocationUri(LOCATION_URI);
        wsdl11Definition.setTargetNamespace("http://bars.kh.ua/soap");
        wsdl11Definition.setSchema(usersSchema);
        return wsdl11Definition;
    }

    @Bean(name = "events")
    public DefaultWsdl11Definition defaultWsdl11DefinitionEvents(XsdSchema eventsSchema){
        DefaultWsdl11Definition wsdl11Definition = new DefaultWsdl11Definition();
        wsdl11Definition.setPortTypeName("eventsPort");
        wsdl11Definition.setLocationUri(LOCATION_URI);
        wsdl11Definition.setTargetNamespace("http://bars.kh.ua/soap");
        wsdl11Definition.setSchema(eventsSchema);
        return wsdl11Definition;
    }

    @Bean
    public XsdSchema usersSchema(){
        return new SimpleXsdSchema(new ClassPathResource("xsd/users.xsd"));
    }

    @Bean
    public XsdSchema eventsSchema(){
        return new SimpleXsdSchema(new ClassPathResource("xsd/events.xsd"));
    }

}
