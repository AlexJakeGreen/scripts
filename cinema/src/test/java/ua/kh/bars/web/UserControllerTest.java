package ua.kh.bars.web;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.security.test.context.support.WithMockUser;
import org.springframework.security.web.FilterChainProxy;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.web.AnnotationConfigWebContextLoader;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.result.MockMvcResultMatchers;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.web.context.WebApplicationContext;
import ua.kh.bars.config.RootConfig;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = {RootConfig.class},
        loader=AnnotationConfigWebContextLoader.class)
@WebAppConfiguration
public class UserControllerTest {

    @Autowired
    private WebApplicationContext webApplicationContext;

    @Autowired
    private FilterChainProxy springSecurityFilterChain;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        this.mockMvc = MockMvcBuilders
                .webAppContextSetup(this.webApplicationContext)
                .build();
    }

    @Test
    public void getUserList() throws Exception {
        this.mockMvc.perform(get("/users")
                .accept(MediaType.TEXT_HTML))
                .andExpect(status().isOk())
                .andExpect(MockMvcResultMatchers.view().name("users/index"));
    }

    @Test
    @WithMockUser(roles = {"ADMIN"})
    public void addUser() throws Exception {
        this.mockMvc.perform(post("/users/new")
                .param("name", "sample name")
                .param("username", "login")
                .param("email", "sample email")
                .param("birthday", "2016-02-01")
                .param("password", "somerandomvalue")
                .param("enabled", "true")
                .param("roles", "ROLE_USER")
                .accept(MediaType.TEXT_HTML))
                .andExpect(status().isOk());
    }

    @Test(expected = Exception.class)
    public void genericException() throws Exception {
        this.mockMvc.perform(get("/users/getGenericException"))
                .andExpect(status().isBadRequest())
                .andExpect(MockMvcResultMatchers.view().name("error"));
    }

}
