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
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.web.context.WebApplicationContext;
import ua.kh.bars.config.RootConfig;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.EventService;
import ua.kh.bars.services.UserAccountService;
import ua.kh.bars.services.UserService;

import static org.junit.Assert.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

/**
 * Created by green on 4/4/16.
 */

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = {RootConfig.class},
        loader=AnnotationConfigWebContextLoader.class)
@WebAppConfiguration
public class UserAccountTest {

    @Autowired
    private WebApplicationContext webApplicationContext;

    @Autowired
    private FilterChainProxy springSecurityFilterChain;

    @Autowired
    private UserAccountService userAccountService;

    @Autowired
    private UserService userService;

    @Autowired
    private EventService eventService;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        this.mockMvc = MockMvcBuilders
                .webAppContextSetup(this.webApplicationContext)
                .build();
    }

    @Test
    @WithMockUser(username = "cat")
    public void checkPrepaidBalanceInHeader() throws Exception {
        User user = userService.findByUsername("cat");
        int balanceBefore = userAccountService.getPrepaid(user);
        this.mockMvc.perform(post("/booking")
                .param("seatNumber", "1")
                .param("eventId", "1")
                .accept(MediaType.TEXT_HTML))
                .andExpect(status().isOk());
        int ticketPrice = eventService.findOne(1L).getPrice();
        int balanceAfter = userAccountService.getPrepaid(user);

        assertEquals(balanceBefore, balanceAfter + ticketPrice);
        assertNotEquals(ticketPrice, 0);
        assertTrue(balanceBefore > 0);
        assertTrue(balanceAfter > 0);
    }


}
