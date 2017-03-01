package ua.kh.bars.config;

import org.dozer.DozerBeanMapper;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.FilterType;
import org.springframework.web.servlet.config.annotation.EnableWebMvc;

/**
 * Created by green on 3/17/16.
 */

@Configuration
@ComponentScan(basePackages = {"ua.kh.bars"},
               excludeFilters = {
                       @ComponentScan.Filter(type = FilterType.ANNOTATION,
                               value = EnableWebMvc.class)
               })
public class RootConfig {

    @Bean
    public DozerBeanMapper dozerBeanMapper() {
        return new DozerBeanMapper();
    }

}
