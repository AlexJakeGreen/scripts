package ua.kh.bars.config;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.encoding.ShaPasswordEncoder;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.method.configuration.EnableGlobalMethodSecurity;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.core.userdetails.UserDetailsService;

/**
 * Created by green on 3/22/16.
 */

@Configuration
@EnableWebSecurity
@EnableGlobalMethodSecurity(securedEnabled = true)
public class SecurityConfig extends WebSecurityConfigurerAdapter {

    @Autowired
    private UserDetailsService userDetailsService;

    @Autowired
    public void registerGlobalAuthentication(AuthenticationManagerBuilder auth)
            throws Exception {
        auth
                .userDetailsService(userDetailsService)
                .passwordEncoder(getShaPasswordEncoder());
    }

    @Override
    protected void configure(HttpSecurity http) throws Exception {

        http.authorizeRequests()
                .antMatchers("/users/**").access("hasRole('ROLE_ADMIN')")
                .antMatchers("/tickets/**").access("hasRole('ROLE_ADMIN')")
                .antMatchers("/events/*").access("hasRole('ROLE_ADMIN') or hasRole('ROLE_USER')")
                .antMatchers("/resources/**", "/**").permitAll();

        http.rememberMe()
                .key("rem-me-key") // token key
                .rememberMeParameter("remember-me") // name of checkbox in html template
                .rememberMeCookieName("my-remember-me") // cookie name
                .tokenValiditySeconds(86400); // 1 day expiration

        http.formLogin()
                .loginPage("/login")
                .loginProcessingUrl("/login")
                .failureUrl("/login-error")
                .usernameParameter("username")
                .passwordParameter("password")
                .defaultSuccessUrl("/events") // TODO: consider showing the profile page
                .permitAll()
                .and().csrf();

        http.logout()
                .logoutUrl("/logout") // This is a POST method here
                .logoutSuccessUrl("/login?logout")
                .invalidateHttpSession(true)
                .permitAll();

        http.csrf().disable();
    }

    @Bean
    public ShaPasswordEncoder getShaPasswordEncoder() {
        return new ShaPasswordEncoder();
    }
}
